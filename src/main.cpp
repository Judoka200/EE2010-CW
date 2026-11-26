#include "card.h"
#include "cardGeneration.h"
#include "enemy.h"
#include "Player.h"
#include "combat.h"


//hi welcome to my cooool ui
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <vector>
 
/* ----- UI DRAWING FUNCTIONS ----- */
 
//big ups https://gist.github.com/Wesxdz/e8667fb20ce5864f758d39685d6547ce
class RoundedRectangle : public sf::Shape {
public:
    explicit RoundedRectangle(const sf::Vector2f& size = {0.0f, 0.0f}, float radius = 0.0f, unsigned int cornerPointCount = 2)
        : m_size(size), m_radius(radius), m_cornerPointCount(cornerPointCount) {
        m_anglePerPoint = 90.0f / static_cast<float>(cornerPointCount);
        update();
    }
 
    void setSize(const sf::Vector2f& size) {
        m_size = size;
        update();
    }
 
    sf::Vector2f getSize() const {
        return m_size;
    }
 
    void setRadius(float radius) {
        m_radius = radius;
        update();
    }
 
    float getRadius() const {
        return m_radius;
    }
 
    void setCornerPointCount(unsigned int points) {
        m_cornerPointCount = points;
        m_anglePerPoint = 90.0f / static_cast<float>(points);
        update();
    }
 
    virtual std::size_t getPointCount() const override {
        return m_cornerPointCount * 4;
    }
 
    virtual sf::Vector2f getPoint(std::size_t index) const override {
        if (m_cornerPointCount == 0 || m_radius == 0.0f) {
            sf::RectangleShape rect(m_size);
            return rect.getPoint(index % 4);
        }
 
        std::size_t corner = index / m_cornerPointCount;
        float angle = 0.0f;
        bool isLastPointOnCorner = (index % m_cornerPointCount == m_cornerPointCount - 1);
 
        if (isLastPointOnCorner) {
            angle = 90.0f;
        } else {
            angle = static_cast<float>(index % m_cornerPointCount) * m_anglePerPoint;
        }
 
        sf::Vector2f centerOffset;
        switch (corner) {
        case 0:
            centerOffset = {m_radius, m_radius};
            angle -= 180.0f;
            break;
        case 1:
            centerOffset = {m_size.x - m_radius, m_radius};
            angle -= 90.0f;
            break;
        case 2:
            centerOffset = {m_size.x - m_radius, m_size.y - m_radius};
            break;
        case 3:
        default:
            centerOffset = {m_radius, m_size.y - m_radius};
            angle += 90.0f;
            break;
        }
 
        float rad = angle * static_cast<float>(M_PI) / 180.0f;
        return centerOffset + sf::Vector2f(std::cos(rad) * m_radius, std::sin(rad) * m_radius);
    }
 
private:
    sf::Vector2f m_size;
    float m_radius;
    unsigned int m_cornerPointCount;
    float m_anglePerPoint;
};

#pragma region SLOT 
//card slot for holding cards in action slots
//c == nullptr -> the slot is empty
//target == -1 -> no enemy slot targeted yet
struct Slot {
    sf::Vector2f pos;
    card* c = nullptr;  // card in slot 
    int target = -1;    // currentl selected target number
    bool arriving = false;  //true while a card is still flying in (slot draws as empty until it lands)
    float arrowT = 0.f;     //0 -> 1 as the target arrow draws itself out
};
 
//determining card slot size (80x80 square u can adapt it)
static const sf::Vector2f kSlotSize(80.f, 80.f); //every slot uses this : )
 #pragma endregion

//the window is split in 2: the game area on the left and the battle log on the right
static const float kWindowW = 1280.f;
static const float kWindowH = 720.f;
static const float kLogWidth = 280.f;
static const float kGameW = kWindowW - kLogWidth;

#pragma region text format
//centering the text onto a still point for cards
//so its in the middle of the card
static void centerText(sf::Text& t, sf::Vector2f center) {
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
    t.setPosition(center);
}
 
//shrinks text so it fits inside maxW (call AFTER centerText so it stays centred)
static void shrinkToFit(sf::Text& t, float maxW) {
    float w = t.getLocalBounds().size.x;
    if (w > maxW) t.setScale({maxW / w, maxW / w});
}
 
//splits a string into lines that fit inside maxWidth (word wrap)
//used by the battle log so long messages dont run off the side
static std::vector<std::string> wrapText(const sf::Font& font, const std::string& s,
                                         unsigned int size, float maxWidth) {
    std::vector<std::string> lines;
    std::istringstream words(s);
    std::string word, line;
    while (words >> word) {
        std::string test = line.empty() ? word : line + " " + word;
        sf::Text t(font, test, size);
        if (t.getLocalBounds().size.x > maxWidth && !line.empty()) {
            lines.push_back(line);
            line = word;
        } else {
            line = test;
        }
    }
    if (!line.empty()) lines.push_back(line);
    if (lines.empty()) lines.push_back("");  //keeps blank entries as a blank line
    return lines;
}
#pragma endregion

#pragma region Battlelog
//draws the battle log panel on the right
//black rectangle, "Battle Log" title at the top, then the log entries underneath
//newest entry is at the bottom, old ones scroll off the top when it fills up
static void drawBattleLog(sf::RenderWindow& window, const sf::Font& font,
                          const std::vector<std::string>& entries) {
    const float pad = 14.f;
    const unsigned int textSize = 16;
    const float lineHeight = 22.f;
 
    //long ugly black rectangle
    sf::RectangleShape panel({kLogWidth, kWindowH});
    panel.setPosition({kGameW, 0.f});
    panel.setFillColor(sf::Color::Black);
    window.draw(panel);
 
    //line on left edge
    sf::RectangleShape edge({3.f, kWindowH});
    edge.setPosition({kGameW, 0.f});
    edge.setFillColor(sf::Color(90, 90, 90));
    window.draw(edge);
 
    //title
    sf::Text title(font, "Battle Log", 26);
    title.setFillColor(sf::Color::White);
    centerText(title, {kGameW + kLogWidth / 2.f, 30.f});
    window.draw(title);
 
    //line under title
    sf::RectangleShape divider({kLogWidth - pad * 2.f, 2.f});
    divider.setPosition({kGameW + pad, 56.f});
    divider.setFillColor(sf::Color(90, 90, 90));
    window.draw(divider);
 
    //wraps every entry then only keeps the last lines that fit
    std::vector<std::string> lines;
    for (const std::string& e : entries) {
        std::vector<std::string> wrapped = wrapText(font, e, textSize, kLogWidth - pad * 2.f);
        lines.insert(lines.end(), wrapped.begin(), wrapped.end());
    }
    const float top = 68.f;
    const int maxLines = static_cast<int>((kWindowH - top - pad) / lineHeight);
    int first = std::max(0, static_cast<int>(lines.size()) - maxLines);
 
    for (int i = first; i < static_cast<int>(lines.size()); ++i) {
        sf::Text t(font, lines[i], textSize);
        t.setFillColor(sf::Color(220, 220, 220));  //CHANGE text colour here
        t.setPosition({kGameW + pad, top + static_cast<float>(i - first) * lineHeight});
        window.draw(t);
    }
}
#pragma endregion

#pragma region anims
//size of a card sitting in your hand @ bottom
//flying cards grow or shrink between this and the action slot size
static const sf::Vector2f kHandCardSize(130.f, 180.f);
static const float kPi = 3.14159265f;
 
//card thats currently animating between your hand and an action slot
//toSlot = true is card travelling to ur action slot from ur hand
//toSlot = false is card travelling back 2 ur hand
//fromPos/fromSize = where it started
//t goes 0 -> 1 over the animation
struct FlyingCard {
    card* c;
    bool toSlot;
    int slot;
    sf::Vector2f fromPos;
    sf::Vector2f fromSize;
    float t = 0.f;
};
 
static float easeOutCubic(float t) {
    float u = 1.f - t;
    return 1.f - u * u * u;
}
 
//draws one card at any size
//used only while a card is flying
static void drawFlyingCard(sf::RenderWindow& window, const sf::Font& font, const sf::Texture& tex,
                           const std::string& name, sf::Vector2f topLeft, sf::Vector2f size, sf::Color tint,
                           sf::Color outlineColor, float outlineThickness) {
    sf::Sprite art(tex);
    art.setPosition(topLeft);
    art.setScale({size.x / static_cast<float>(tex.getSize().x),
                  size.y / static_cast<float>(tex.getSize().y)});
    art.setColor(tint);
    window.draw(art);
 
    RoundedRectangle shape(size, 5.f, 8);
    shape.setPosition(topLeft);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineThickness(outlineThickness);
    shape.setOutlineColor(outlineColor);
    window.draw(shape);
 
    //label shrinks with the card (20 is the card in hand size)
    unsigned int fontSize = std::max(12u, static_cast<unsigned int>(20.f * size.x / kHandCardSize.x));
    sf::Text label(font, name, fontSize);
    label.setFillColor(sf::Color::Black);
    centerText(label, {topLeft.x + size.x / 2.f, topLeft.y + 25.f * size.y / kHandCardSize.y});
    shrinkToFit(label, size.x - 16.f * size.x / kHandCardSize.x);
    window.draw(label);
}
#pragma endregion


//loops over the slots and builds the rectangle around each one
//basically this is how it chooses what slot ur hovering
static int slotAt(const std::vector<Slot>& slots, sf::Vector2f p) {
    for (int i = 0; i < static_cast<int>(slots.size()); ++i) {
        if (sf::FloatRect(slots[i].pos, kSlotSize).contains(p)) return i;
    }
    return -1;
}
 
#pragma region Draw info
//draws the. health bar. obv
static void drawHealthBar(sf::RenderWindow& window, const sf::Font& font,
                          sf::Vector2f topLeft, float width, int hp, float shownHp, int maxHp) {
    const float height = 22.f;
 
    //backing for the health bar
    sf::RectangleShape back({width, height});
    back.setPosition(topLeft);
    back.setFillColor(sf::Color(60, 60, 60));
    back.setOutlineThickness(2.f);
    back.setOutlineColor(sf::Color::Black);
    window.draw(back);
 
    //red ghost bar = the hp you just lost (gradual drain)
    //shownHp chases the real hp (see the animate section in the loop)
    sf::RectangleShape trail({width * (shownHp / static_cast<float>(maxHp)), height});
    trail.setPosition(topLeft);
    trail.setFillColor(sf::Color(230, 80, 80));
    window.draw(trail);
 
    //ratio = % of the bar i fill with color
    float ratio = static_cast<float>(hp) / static_cast<float>(maxHp);
    sf::RectangleShape fill({width * ratio, height});
    fill.setPosition(topLeft);
    fill.setFillColor(sf::Color(90, 200, 90));
    window.draw(fill);
 
    //label on the health bar showing health eg. '10/30hp'
    sf::Text label(font, std::to_string(hp) + "/" + std::to_string(maxHp) + "hp", 16);
    label.setFillColor(sf::Color::White);
    label.setOutlineColor(sf::Color::Black);
    label.setOutlineThickness(2.f);
    centerText(label, {topLeft.x + width / 2.f, topLeft.y + height / 2.f - 2.f});
    window.draw(label);
}
 
//small square status icons (poison, strength, slash etc.) shown in a row under the hp bar
//call it only by [name], not [name].png
static const float kIconSize = 28.f;  //icons are sqinched to this square size
static const float kIconGap = 4.f;
static void drawIcons(sf::RenderWindow& window, const std::map<std::string, sf::Texture>& textures,
                      const std::vector<std::string>& icons, float centerX, float top) {
    if (icons.empty()) return;
    float n = static_cast<float>(icons.size());
    float totalWidth = n * kIconSize + (n - 1.f) * kIconGap;
    float x = centerX - totalWidth / 2.f;  //row is centered under the hp bar
    for (const std::string& name : icons) {
        auto it = textures.find(name);
        if (it == textures.end()) continue;
 
        //dark backing so light icons still read on the cave bg
        sf::RectangleShape backing({kIconSize, kIconSize});
        backing.setPosition({x, top});
        backing.setFillColor(sf::Color(0, 0, 0, 140));
        backing.setOutlineThickness(2.f);
        backing.setOutlineColor(sf::Color::Black);
        window.draw(backing);
 
        sf::Sprite icon(it->second);
        icon.setPosition({x, top});
        icon.setScale({kIconSize / static_cast<float>(it->second.getSize().x),
                       kIconSize / static_cast<float>(it->second.getSize().y)});
        window.draw(icon);
        x += kIconSize + kIconGap;
    }
}
 
//it draws the placeholder for the character
//CHANGE: remove this when the actual drawings for the character are made and sub those in
static void drawCharacter(sf::RenderWindow& window, const sf::Font& font,
                          sf::Vector2f topLeft, sf::Color colour,
                          const std::string& name, int hp, float shownHp, int maxHp,
                          int block,
                          const std::vector<std::string>& icons,
                          const std::map<std::string, sf::Texture>& iconTextures) {

    const sf::Vector2f size(140.f, 220.f);
    sf::RectangleShape box(size);
    box.setPosition(topLeft);
    box.setFillColor(colour);
    box.setOutlineThickness(3.f);
    box.setOutlineColor(sf::Color::Black);
    window.draw(box);

    const float centerX = topLeft.x + size.x / 2.f;
    const float iconTop = topLeft.y - 8.f - kIconSize;  //icon row sits just above the character
    const float barTop = iconTop - 6.f - 22.f;          //22 = health bar height

    // draws the name of the character above the hp bar
    sf::Text nameText(font, name, 22);
    nameText.setFillColor(sf::Color::White);
    nameText.setOutlineColor(sf::Color::Black);
    nameText.setOutlineThickness(3.f);
    centerText(nameText, {centerX, barTop - 18.f});
    window.draw(nameText);

    //health bar a little wider than the character    
    drawHealthBar(window, font, {topLeft.x - 10.f, barTop},
                  size.x + 20.f, hp, shownHp, maxHp);

    // Block value
    sf::Text blockText(font, "Block: " + std::to_string(block), 18);
    blockText.setFillColor(sf::Color(100, 180, 255));
    blockText.setOutlineColor(sf::Color::Black);
    blockText.setOutlineThickness(2.f);

    centerText(blockText, {centerX, barTop + 32.f});
    window.draw(blockText);

    // Status icons
    drawIcons(window, iconTextures, icons, centerX, iconTop);
}

//draws an action slot (with the name of the card in it)
static void drawSlot(sf::RenderWindow& window, const sf::Font& font, const Slot& slot,
                     sf::Color base, bool selected, bool highlighted) {
    sf::RectangleShape box(kSlotSize);
    box.setPosition(slot.pos);
 
    //if the slot is filled, the color is solid and shows its full
    bool filled = slot.c != nullptr && !slot.arriving;  //arriving = card still flying in, so show it empty
    sf::Color fill = base;
    //if the slot is not filled, the color is transparent and shows its empty
    if (!filled) fill.a = 110;
    box.setFillColor(fill);
 
    //hilights the selected slot yellow if you click it otherwise black
    if (selected || highlighted) {
        box.setOutlineThickness(5.f);
        box.setOutlineColor(sf::Color(255, 200, 40));
    } else {
        box.setOutlineThickness(3.f);
        box.setOutlineColor(filled ? sf::Color::Black : base);
    }
    window.draw(box);
 
    //card title in the slot, shrunk so long names stay inside the box
    //CHANGE to a square card sprite when art is available
    if (filled) {
        sf::Text label(font, slot.c->getName()+" "+std::to_string(slot.c->iD()), 16);
        label.setFillColor(sf::Color::White);
        label.setOutlineColor(sf::Color::Black);
        label.setOutlineThickness(2.f);
        centerText(label, slot.pos + kSlotSize / 2.f);
        shrinkToFit(label, kSlotSize.x - 10.f);
        window.draw(label);
    }
}
#pragma endregion

//draws an arced arrow above everything so u can see what each thing is connected to
//progress of the animation is how much of the arrow is drawn (0 = nothing, 1 = full arrow)
//the arrowhead remains on the leading edge of the drawn arrow while it grows
static void drawArcArrow(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end, sf::Color color,
                         float progress = 1.f) {
    progress = std::min(1.f, progress);
    if (progress <= 0.f) return;
    const float headLen = 22.f;
    const float headHalfWidth = 12.f;
    const float thickness = 5.f;
    const float outline = 2.f;
    const int segments = 48;
 
    //making sure the midway point is above everything so it doesnt arc into it
    sf::Vector2f ctrl((start.x + end.x) / 2.f, std::min(start.y, end.y) - 160.f);
    sf::Vector2f d = end - ctrl;
    float len = std::sqrt(d.x * d.x + d.y * d.y);
    d /= len;
    //line ends at arrowhead
    sf::Vector2f lineEnd = end - d * headLen;
 
    //quadratic bezier curve for a smooth curve of the arrow over everything
    std::vector<sf::Vector2f> pts;
    for (int i = 0; i <= segments; ++i) {
        float t = progress * static_cast<float>(i) / segments;  //only samples the part drawn so far
        float u = 1.f - t;
        pts.push_back(start * (u * u) + ctrl * (2.f * u * t) + lineEnd * (t * t)); //curve formula
    }
 
    //lines are not allowed to be thick in sfml so u have to do this
    //uses strips to thicken it out like a ribbon
    auto drawStrip = [&](float width, sf::Color c) {
        sf::VertexArray strip(sf::PrimitiveType::TriangleStrip, pts.size() * 2);
        for (std::size_t i = 0; i < pts.size(); ++i) {
            sf::Vector2f a = pts[i == 0 ? 0 : i - 1];
            sf::Vector2f b = pts[std::min(i + 1, pts.size() - 1)];
            sf::Vector2f dir = b - a;
            float l = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (l > 0.0001f) dir /= l; else dir = d;  //guards the first few frames when the arrow is tiny
            sf::Vector2f normal(-dir.y, dir.x);
 
            strip[2 * i].position = pts[i] + normal * (width / 2.f);
            strip[2 * i].color = c;
            strip[2 * i + 1].position = pts[i] - normal * (width / 2.f);
            strip[2 * i + 1].color = c;
        }
        window.draw(strip);
    };
    drawStrip(thickness + outline * 2.f, sf::Color(20, 20, 20));  // dark outline underneath
    drawStrip(thickness, color);
 
    //triangular arrowhead
    //while growing, the head sits on the end of the drawn line and points along the curve there
    //it also scales up over the first bit so it doesnt just pop in
    sf::Vector2f tip = end;
    sf::Vector2f base = lineEnd;
    sf::Vector2f headDir = d;
    float headScale = 1.f;
    if (progress < 1.f) {
        float t = progress;
        sf::Vector2f tangent = (ctrl - start) * (2.f * (1.f - t)) + (lineEnd - ctrl) * (2.f * t);
        float tl = std::sqrt(tangent.x * tangent.x + tangent.y * tangent.y);
        if (tl > 0.0001f) headDir = tangent / tl;
        headScale = std::min(1.f, progress / 0.15f);  //0.15 = how quickly the head grows to full size
        base = pts.back();
        tip = base + headDir * (headLen * headScale);
    }
    sf::Vector2f perp(-headDir.y, headDir.x);
    sf::ConvexShape head(3);
    head.setPoint(0, tip);
    head.setPoint(1, base + perp * (headHalfWidth * headScale));
    head.setPoint(2, base - perp * (headHalfWidth * headScale));
    head.setFillColor(color);
    head.setOutlineThickness(outline);
    head.setOutlineColor(sf::Color(20, 20, 20));
    window.draw(head);
}
 





//main
int main() {
 
    //seed rand() once here (drawCard() in playerDeck.cpp should NOT call srand itself)
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
 
    //initialise player and enemy
    Player p(16);
    enemy e("grunt", 100);
 
    p.deck().load("assets/card_lists/cards.txt");
    e.deck().load("assets/card_lists/enemyCards.txt");
    for (int i = 0; i < 5; ++i) p.deck().drawCard();
 
    p.deck().show();
 
    // create game window
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Card Game UI");
    window.setFramerateLimit(60);
    sf::Font font;
    if (!font.openFromFile("assets/Snowbell-Wp4g9.ttf")) {
        std::cerr << "Could not load font. Check the path in main.cpp\n";
        return 1;
    }
 
    //load background texture (cave bg scaled to the game area, not under the log)
    sf::Texture caveTexture;
    if (!caveTexture.loadFromFile("assets/cave.png")) {
        std::cerr << "Could not load assets/cave.png. Make sure it is copied next to the executable.\n";
        return 1;
    }
    caveTexture.setSmooth(true);
    sf::Sprite cave(caveTexture);
    cave.setScale({kGameW / static_cast<float>(caveTexture.getSize().x),
                   kWindowH / static_cast<float>(caveTexture.getSize().y)});
 
    //card backgrounds (card-bg1.png, card-bg2.png, card-bg3.png)
    //each card gets one of these picked at random
    //keep this vector alive for the whole program, dont resize it after loading
    std::vector<sf::Texture> cardTextures(3);
    for (int i = 0; i < 3; ++i) {
        std::string path = "assets/card-textures/card-bg" + std::to_string(i + 1) + ".png";
        if (!cardTextures[i].loadFromFile(path)) {
            std::cerr << "Could not load " << path << ". Make sure it is copied next to the executable.\n";
            return 1;
        }
        cardTextures[i].setSmooth(true);
    }
 
    //status icons: assets/effects/*.png and assets/attack-types/*.png
    //stored by file name (no .png) so u call them by name
    //missing files are skipped instead of stopping the game
    std::map<std::string, sf::Texture> iconTextures;
    const std::vector<std::pair<std::string, std::string>> iconFiles = {
        {"effects", "bleed"},   {"effects", "burn"},     {"effects", "madness"},
        {"effects", "minushp"}, {"effects", "plague"},   {"effects", "plushp"},
        {"effects", "poison"},  {"effects", "strength"}, {"effects", "vulnerable"},
        {"attack-types", "defend"}, {"attack-types", "slash"},
        {"attack-types", "stab"},   {"attack-types", "strike"},
    };
    for (const auto& f : iconFiles) {
        std::string path = "assets/" + f.first + "/" + f.second + ".png";
        if (iconTextures[f.second].loadFromFile(path)) {
            iconTextures[f.second].setSmooth(true);
        } else {
            std::cerr << "Could not load " << path << " (icon skipped)\n";
            iconTextures.erase(f.second);
        }
    }

    std::vector<std::string> iconNames;  //only used by the test keys below
    for (const auto& entry : iconTextures) iconNames.push_back(entry.first);
 

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> pickBg(0, 2);
    std::map<int, int> cardBg;  //card id -> which background (0-2), picked once so it sticks
    auto bgFor = [&](card* c) {
        int id = c->iD();
        if (!cardBg.count(id)) cardBg[id] = pickBg(rng);
        return cardBg[id];
    };
    #pragma region pos, slot, stat definitions 
    // load player an enemy health values
    int maxEnergy = p.getEnergy();
    int turnNumber = 1;
    int playerMaxHp = p.getMaxHealth();
    int enemyMaxHp = e.getMaxHealth();
    float playerShownHp = static_cast<float>(p.getHealth());  //ghost bars start full
    float enemyShownHp = static_cast<float>(e.getHealth());
 
    std::vector<std::string> playerIcons;
    std::vector<std::string> enemyIcons;
    std::size_t nextTestIcon = 0;

    //BATTLE LOG TEXT
    //each string is one entry in the log, newest at the bottom
    //eg. battleLog.push_back("Player used Card 3 on Enemy for 10 damage");
    std::vector<std::string> battleLog;
 
    //character positions (squeezed in so everything fits left of the log)
    const sf::Vector2f playerPos(130.f, 280.f);
    const sf::Vector2f enemyPos(730.f, 280.f);
    const sf::Color blue(70, 120, 220);
    const sf::Color red(220, 70, 70);
 
    //calls 3 action slots above each char
    const float slotY = 100.f;
    const float slotGap = 20.f;
    auto makeSlots = [&](float centerX) {
        std::vector<Slot> slots(3);
        float totalWidth = 3.f * kSlotSize.x + 2.f * slotGap;
        for (int i = 0; i < 3; ++i) {
            slots[i].pos = {centerX - totalWidth / 2.f + i * (kSlotSize.x + slotGap), slotY};
        }
        return slots;
    };
 
    //generate player and enemy slots
    std::vector<Slot> playerSlots = makeSlots(playerPos.x + 70.f); //70.f is horizontal offset
    std::vector<Slot> enemySlots = makeSlots(enemyPos.x + 70.f);
    int selectedCard = -1;  //card thats selected (-1=none)
    int selectedSlot = -1;  //slot thats selected (-1=none)
 
    //determines the color of the arrows drawn from your action slot to theirs
    //1st arrow = yellow, 2nd = red, 3rd = orange
    std::vector<int> arrowOrder;
    const sf::Color arrowColors[3] = {
        sf::Color(255, 230, 120),  // yellow
        sf::Color(235, 50, 50),    // red
        sf::Color(255, 150, 30),   // orange
    };
    #pragma endregion

    #pragma region  En turn
    auto enemyTurn = [&](enemy& en) {
        for (int i = 0; i < 3; i++) {
            if (enemySlots[i].c != nullptr) {
                continue;
            }
 
            card* enemyCard = en.enemyact();
 
            if (enemyCard == nullptr) {
                break;
            }
            enemySlots[i].c = enemyCard;
            en.deck().returnCardToDeck(enemyCard);
            
        }
    };
    #pragma endregion
        enemyTurn(e);
 
    std::vector<card*> hand;    //shortcut variale  
    // refresh the hand vector based on player's hand
    auto syncHand = [&]() {
        hand = p.deck().getHand();
    };
    syncHand();

    // turn logic 
    auto resolveTurn = [&](){   
        bool enemyUsed[3] = {false, false, false}; //enemies three action slots
    const std::string pName = "Player";
    const std::string eName = e.getname();

    // trigger player slots form left to right 
    for (int i = 0; i < 3; ++i) {
        card* playerCard = playerSlots[i].c;
        if (!playerCard) continue;

        int target = playerSlots[i].target;
        card* enemyCard = (target >= 0 && target < 3) ? enemySlots[target].c : nullptr;

        // battle log stuffs 
        // damage calc etc is done in resolveClash()
        if (enemyCard) {
            battleLog.push_back(resolveClash(playerCard, enemyCard, p, e, pName, eName));
            enemyUsed[target] = true;
        } else {
            battleLog.push_back(resolveUnopposed(playerCard, p, e, pName, eName));
        }
    }

    // untargeted enemy cards 
    for (int i = 0; i < 3; ++i) {
        if (enemySlots[i].c && !enemyUsed[i])
            battleLog.push_back(resolveUnopposed(enemySlots[i].c, e, p, eName, pName));
    }

    // 3. clean up: played cards go back to the deck, slots empty
    // send used cards back to deck 
    for (Slot& slot : playerSlots) {
        if (slot.c) {
            p.deck().returnCardToDeck(slot.c);
        }
        slot.c = nullptr; slot.target = -1; slot.arrowT = 0.f; slot.arriving = false;
    }

    for (Slot& slot : enemySlots){
        slot.c = nullptr; slot.target = -1; 
    }
    arrowOrder.clear();
    selectedCard = -1;  //reset to unselected 
    selectedSlot = -1;

    //start of next turn prep
    e.activateTurnStart();   // poison etc. + resets enemy block
    p.turnStart();           // energy back to 3, status effects, draws a card
    enemyTurn(e);            // refill enemy slots
    syncHand();
    };

 
    const sf::Vector2f cardSize = kHandCardSize;
    const float cardStep = 75.f;   //distance between card left edges (reduce for more overlap)
    const float cardLift = 40.f;   //how far hovered card rises
    const float handY = 720.f - cardSize.y - 10.f;
    const float handCenterX = 450.f;  //hand sits between the energy circle and the turn button
 
    auto handStartX = [&]() {
        float w = cardStep * (static_cast<float>(hand.size()) - 1.f) + cardSize.x;
        return handCenterX - w / 2.f;
    };
 
    //checks which cards under the point (right to left)
    //to determine which card clicked
    auto cardAt = [&](sf::Vector2f pt) {
        for (int i = static_cast<int>(hand.size()) - 1; i >= 0; --i) {
            sf::FloatRect r({handStartX() + i * cardStep, handY - cardLift}, {cardSize.x, cardSize.y + cardLift});
            if (r.contains(pt)) return i;
        }
        return -1;
    };
 
    //begin turn button
    const sf::Vector2f turnButtonPos(740.f, 610.f);
    const sf::Vector2f turnButtonSize(240.f, 70.f);
    sf::RectangleShape turnButton(turnButtonSize);
    turnButton.setPosition(turnButtonPos);
    turnButton.setOutlineThickness(3.f);
    turnButton.setOutlineColor(sf::Color::Black);
 
    //energy circle
    const float energyRadius = 50.f;
    const sf::Vector2f energyPos(40.f, 540.f + (70.f - energyRadius));
    sf::CircleShape energyCircle(energyRadius);
    energyCircle.setPosition(energyPos);
    energyCircle.setFillColor(sf::Color(255, 244, 163));  // pastel yellow
    energyCircle.setOutlineThickness(4.f);
    energyCircle.setOutlineColor(sf::Color(30, 30, 30));
 
    //card lift animation
    //each card remembers the raise height its @
    //every frame it moves a fraction of the way to where it should be
    std::map<card*, float> liftAmount;
    const float cardAnimSpeed = 18.f;  //increase num for faster
    sf::Clock clock;
 
    //card flight animation
    //playing a card = it flies from your hand and SLAMS! into the slot
    //right clicking a played card = returns to ur hand = animated to come back
    std::vector<FlyingCard> flights;
    const float slamTime = 0.32f;     //seconds for hand -> slot
    const float returnTime = 0.28f;   //seconds for slot -> hand
    const float arrowTime = 0.3f;     //seconds for a target arrow to draw out
 
    //true while this card is mid-flight back 2 ur hand
    //so hand skips drawing it so it isnt doubled
    auto isReturning = [&](card* c) {
        for (const FlyingCard& f : flights) {
            if (!f.toSlot && f.c == c) return true;
        }
        return false;
    };
 
    while (window.isOpen()) {
        //events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        
            //CHANGE
            //test keys for the health bar drain
            //DELETE THESE WHEN U LINK IT UP TO REAL DAMAGE
            //D = damages enemy, A = damage player, H = restores both 2 full
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {

                //test keys for the icons DELETE THESE TOO
                //E = adds the next icon to the enemy, P = adds it to the player, C = clears all icons
                if (!iconNames.empty() && (key->code == sf::Keyboard::Key::E || key->code == sf::Keyboard::Key::P)) {
                    std::vector<std::string>& target = (key->code == sf::Keyboard::Key::E) ? enemyIcons : playerIcons;
                    if (target.size() < 6) target.push_back(iconNames[nextTestIcon++ % iconNames.size()]);
                }
                if (key->code == sf::Keyboard::Key::C) {
                    std::cout<< "clear\n";
                    playerIcons.clear();
                    enemyIcons.clear();
                }
            }



            if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f m = window.mapPixelToCoords(click->position);
                if (click->button == sf::Mouse::Button::Left) {
                    if (turnButton.getGlobalBounds().contains(m)) {

                        // Check that the player has actually played a card
                        resolveTurn();
                        turnNumber++;
                    } else {
                        int c = cardAt(m);
                        int ps = slotAt(playerSlots, m);
                        int es = slotAt(enemySlots, m);
 
                        if (c != -1) {
                            //if this card was still gliding back to your hand just let it land instantly
                            card* clicked = hand[c];
                            flights.erase(std::remove_if(flights.begin(), flights.end(),
                                              [&](const FlyingCard& f) { return !f.toSlot && f.c == clicked; }),
                                          flights.end());
                            //clicking a card selects it (right click for deselect)
                            selectedCard = (c == selectedCard) ? -1 : c;
                            selectedSlot = -1;


                        } else if (ps != -1) {
                            if (playerSlots[ps].arriving) {
                                //card is still flying into this slot, ignore clicks on it

                            } else if (selectedCard != -1 && playerSlots[ps].c == nullptr && p.getEnergy() >= hand[selectedCard]->getCost()) {
                                //works out where the card is drawn right now (with its lift + grow)
                                //so the flight starts exactly from there
                                p.removEnergy(hand[selectedCard]->getCost());    //remove energy 
                                

                                card* played = hand[selectedCard];
                                float lift = liftAmount[played];
                                sf::Vector2f sz = cardSize * (1.f + 0.1f * (lift / cardLift));
                                sf::Vector2f from(handStartX() + selectedCard * cardStep - (sz.x - cardSize.x) / 2.f,
                                                  handY - lift - (sz.y - cardSize.y));
                                flights.push_back(FlyingCard{played, true, ps, from, sz, 0.f});
                                liftAmount[played] = 0.f;  //so it starts at rest if it comes back later
 
                                //places the card into a box if it is empty
                                playerSlots[ps].c = played;
                                playerSlots[ps].arriving = true;  //slot looks empty until the card lands
                                hand.erase(hand.begin() + selectedCard);
                                selectedCard = -1;
                                selectedSlot = -1;
                            } else if (playerSlots[ps].c != nullptr) {
                                //selects a clicked filled box so u can attack enemy box
                                selectedSlot = (ps == selectedSlot) ? -1 : ps;
                                selectedCard = -1;
                            }
                        } else if (es != -1) {
                            //clicking an enemy slot selects it if ur holding a filled action box
                            if (selectedSlot != -1 && playerSlots[selectedSlot].c != nullptr) {
                                playerSlots[selectedSlot].target = es;
                                playerSlots[selectedSlot].arrowT = 0.f;  //arrow draws itself out again (also when re-aiming)
                                //rmbr when this arrow was first drawn (re-aiming keeps its color)
                                if (std::find(arrowOrder.begin(), arrowOrder.end(), selectedSlot) == arrowOrder.end()) {
                                    arrowOrder.push_back(selectedSlot);
                                }
                                selectedSlot = -1;
                            }
                        } else {
                            selectedSlot = -1;  //clicked empty space
                            selectedCard = -1;
                        }
                    }
                } else if (click->button == sf::Mouse::Button::Right) {
                    //right clicking returns it to your hand n undoes the action box
                    int ps = slotAt(playerSlots, m);
                    if (ps != -1 && playerSlots[ps].c != nullptr && !playerSlots[ps].arriving) {
                        //card goes from the action slot back to its spot in your hand
                        card* returned = playerSlots[ps].c;
                        flights.push_back(FlyingCard{returned, false, ps, playerSlots[ps].pos, kSlotSize, 0.f});
                        liftAmount[returned] = 0.f;
                        hand.push_back(returned);
                        p.setEnergy(p.getEnergy() + playerSlots[ps].c->getCost());
                        //sort by card id (sorting the raw pointers would order by memory address)
                        std::sort(hand.begin(), hand.end(),
                                  [](card* a, card* b) { return a->iD() < b->iD(); });
                        playerSlots[ps].c = nullptr;
                        playerSlots[ps].target = -1;
                        playerSlots[ps].arrowT = 0.f;
                        arrowOrder.erase(std::remove(arrowOrder.begin(), arrowOrder.end(), ps), arrowOrder.end());
                        if (selectedSlot == ps) selectedSlot = -1;
                        selectedCard = -1;  //removes card selection
                    }
                }
            }
        }
 
        //hover state KEEP ORDER HERE
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        int hoveredCard = cardAt(mouse);
        bool overButton = turnButton.getGlobalBounds().contains(mouse);
        bool targeting = selectedSlot != -1 && playerSlots[selectedSlot].c != nullptr;
        int hoveredEnemySlot = targeting ? slotAt(enemySlots, mouse) : -1;
        int hoveredPlayerSlot = -1;  //hilight empty blue boxes while a card is selected
        if (selectedCard != -1) {
            int h = slotAt(playerSlots, mouse);
            if (h != -1 && playerSlots[h].c == nullptr) hoveredPlayerSlot = h;
        }
 
        //animate card lift
        float dt = std::min(clock.restart().asSeconds(), 0.05f);  //clamped so a lag spike doesnt jump
        float blend = 1.f - std::exp(-cardAnimSpeed * dt);        //frame rate independent easing
        for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
            bool raisedNow = (i == hoveredCard || i == selectedCard);
            float liftTarget = raisedNow ? cardLift : 0.f;
            float& current = liftAmount[hand[i]];  //starts at 0 the first time a card is seen
            current += (liftTarget - current) * blend;
        }
 
        //animate flying cards
        for (std::size_t i = 0; i < flights.size();) {
            FlyingCard& f = flights[i];
            f.t += dt / (f.toSlot ? slamTime : returnTime);
            if (f.t >= 1.f) {
                if (f.toSlot) {
                    playerSlots[f.slot].arriving = false;  //landed: slot fills in
                }
                flights.erase(flights.begin() + static_cast<int>(i));
            } else {
                ++i;
            }
        }
 
        //animate target arrows drawing out from the player slot
        for (Slot& s : playerSlots) {
            if (s.c != nullptr && s.target != -1 && s.arrowT < 1.f) {
                s.arrowT = std::min(1.f, s.arrowT + dt / arrowTime);
            }
        }
 
        //animate health bar drain
        //green bar drops instantly, red ghost bar eases down behind it
        auto drain = [&](float& shown, int hp) {
            if (shown < hp) shown = static_cast<float>(hp);       //healing: ghost snaps up
            shown += (hp - shown) * (1.f - std::exp(-3.f * dt));  //damage: ghost eases down (3 = speed)
            if (std::abs(shown - hp) < 0.05f) shown = static_cast<float>(hp);
        };
        drain(playerShownHp, p.getHealth());
        drain(enemyShownHp, e.getHealth());
 
        window.clear(sf::Color::Black);
        window.draw(cave);
 
        // CHANGE hint text to advise player
        std::string hint = "click a card in your hand to play it !";
        if (selectedCard != -1) {
            hint = "click one of your action boxes to play the card !";
        } else if (targeting) {
            hint = "click an enemy card to target it with yours !";
        }
        sf::Text hintText(font, hint, 20);
        hintText.setFillColor(sf::Color::White);
        hintText.setOutlineColor(sf::Color::Black);
        hintText.setOutlineThickness(3.f);
        hintText.setPosition({20.f, 15.f});
        window.draw(hintText);
 
        //drawing characters (status icons come from the real poison/madness/burning/bleed/block values)
        drawCharacter(window, font, playerPos, blue, "Player", p.getHealth(), playerShownHp, playerMaxHp,p.getBlock(),
                      playerIcons, iconTextures);
        drawCharacter(window, font, enemyPos, red, e.getname(), e.getHealth(), enemyShownHp, enemyMaxHp,e.getBlock(),
                      enemyIcons, iconTextures);
 
        //action slots
        for (int i = 0; i < 3; ++i) {
            drawSlot(window, font, enemySlots[i], red, false, i == hoveredEnemySlot);
            drawSlot(window, font, playerSlots[i], blue, i == selectedSlot, i == hoveredPlayerSlot);
        
        }
 
        //your hand
        //show order normal cards < selected card < hovered card
        float startX = handStartX();
        for (int pass = 0; pass < 3; ++pass) {
            for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
                bool isHovered = (i == hoveredCard);
                bool isSelected = (i == selectedCard);
                int priority = isHovered ? 2 : (isSelected ? 1 : 0);
                if (priority != pass) continue;
                if (isReturning(hand[i])) continue;  //its flying back, drawn below instead
 
                bool raised = isHovered || isSelected;
                float y = handY - liftAmount[hand[i]];  //animated height instead of a hard jump
 
                //card grows up to 10% bigger as it rises
                //grows upwards so the bottom edge stays put
                float t = liftAmount[hand[i]] / cardLift;  //0 = resting, 1 = fully raised
                float scale = 1.f + 0.1f * t;
                sf::Vector2f size = cardSize * scale;
                float top = y - (size.y - cardSize.y);
 
                //rounded corners (size, corner radius, points per corner)
                RoundedRectangle cardShape(size, 5.f, 8);
                cardShape.setPosition({startX + i * cardStep - (size.x - cardSize.x) / 2.f, top});
 
                //background art for this card (random pick, stays with the card id)
                //stretched 2 cards current animated size
                const sf::Texture& tex = cardTextures[bgFor(hand[i])];
                sf::Sprite art(tex);
                art.setPosition(cardShape.getPosition());
                art.setScale({size.x / static_cast<float>(tex.getSize().x),
                              size.y / static_cast<float>(tex.getSize().y)});
                //tint keeps the hover/selected feedback (white = full brightness)
                art.setColor(isSelected ? sf::Color(255, 255, 235)
                                        : (isHovered ? sf::Color(235, 235, 235) : sf::Color(210, 210, 210)));
                window.draw(art);
 
                //the rounded shape now only draws the outline on top
                cardShape.setFillColor(sf::Color::Transparent);
                cardShape.setOutlineThickness(isSelected ? 6.f : (isHovered ? 5.f : 3.f));
                cardShape.setOutlineColor(raised ? sf::Color(255, 200, 40) : sf::Color(30, 30, 30));
                window.draw(cardShape);
 
                float centerX = startX + i * cardStep + cardSize.x / 2.f;
 
                // card name (top of card), shrunk so long names stay inside the card
                sf::Text nameText(font, hand[i]->getName(), 20);
                nameText.setFillColor(sf::Color::Black);
                centerText(nameText, {centerX, top + 25.f});
                shrinkToFit(nameText, cardSize.x - 16.f);
                window.draw(nameText);
 
                // card id (just below the name)
                sf::Text idText(font, "ID: " + std::to_string(hand[i]->iD()), 14);
                idText.setFillColor(sf::Color(60, 60, 60));
                centerText(idText, {centerX, top + 50.f});
                window.draw(idText);
 
                // attack value (below the id)
                sf::Text attackDmg(font, "Score: " + std::to_string(hand[i]->getAttack()), 14);
                attackDmg.setFillColor(sf::Color(60, 60, 60));
                centerText(attackDmg, {centerX, top + 70.f});
                window.draw(attackDmg);
            }
        }
 
        //flying cards (drawn over the hand and slots)
        for (const FlyingCard& f : flights) {
            float t = std::min(f.t, 1.f);
            sf::Vector2f fromCenter = f.fromPos + f.fromSize / 2.f;
            sf::Vector2f toCenter, toSize;
            sf::Color tint, outlineColor;
            float outlineThickness, pr, bump;
 
            if (f.toSlot) {
                //slam so its like a quick start then it speeds up into the slot
                //bump makes the card bigger toward the camera mid-flight then go back down
                toSize = kSlotSize;
                toCenter = playerSlots[f.slot].pos + kSlotSize / 2.f;
                pr = 0.25f * t + 0.75f * t * t * t;
                bump = 1.f + 0.35f * std::sin(kPi * t);
                tint = sf::Color(255, 255, 235);
                outlineColor = sf::Color(255, 200, 40);
                outlineThickness = 5.f;
            } else {
                //return js makes it return to ur hand w no bump
                auto it = std::find(hand.begin(), hand.end(), f.c);
                if (it == hand.end()) continue;
                int idx = static_cast<int>(it - hand.begin());
                toSize = cardSize;
                toCenter = {startX + idx * cardStep + cardSize.x / 2.f, handY + cardSize.y / 2.f};
                pr = easeOutCubic(t);
                bump = 1.f + 0.15f * std::sin(kPi * t);
                tint = sf::Color(210, 210, 210);
                outlineColor = sf::Color(30, 30, 30);
                outlineThickness = 3.f;
            }
 
            //card scales from the center
            sf::Vector2f size = (f.fromSize + (toSize - f.fromSize) * pr) * bump;
            sf::Vector2f center = fromCenter + (toCenter - fromCenter) * pr;
            drawFlyingCard(window, font, cardTextures[bgFor(f.c)], f.c->getName(), center - size / 2.f, size,
                           tint, outlineColor, outlineThickness);
        }
 
        //energy circle
        window.draw(energyCircle);
        sf::Text energyText(font, std::to_string(p.getEnergy()) + "/" + std::to_string(maxEnergy),
                            static_cast<unsigned int>(energyRadius * 48.f / 70.f));
        energyText.setFillColor(sf::Color::Black);
        centerText(energyText, energyPos + sf::Vector2f(energyRadius, energyRadius));
        window.draw(energyText);
 
        //begin turn button
        turnButton.setFillColor(overButton ? sf::Color(255, 255, 255) : sf::Color(235, 235, 235));
        window.draw(turnButton);
        sf::Text turnText(font, "Begin Turn " + std::to_string(turnNumber), 28);
        turnText.setFillColor(sf::Color::Black);
        centerText(turnText, turnButtonPos + turnButtonSize / 2.f);
        window.draw(turnText);
 
        //arrows in drawn order (yellow, red, orange)
        for (std::size_t k = 0; k < arrowOrder.size(); ++k) {
            const Slot& s = playerSlots[arrowOrder[k]];
            if (s.c != nullptr && s.target != -1) {
                sf::Vector2f from(s.pos.x + kSlotSize.x / 2.f, s.pos.y - 2.f);
                sf::Vector2f to(enemySlots[s.target].pos.x + kSlotSize.x / 2.f,
                                enemySlots[s.target].pos.y - 4.f);
                drawArcArrow(window, from, to, arrowColors[k % 3], easeOutCubic(s.arrowT));  //eased so it shoots out fast then settles
            }
        }
 
        //battle log goes last //do not move this it needs to go last
        drawBattleLog(window, font, battleLog);
 
        window.display();
    }
    return 0;
}
