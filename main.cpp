//hi welcome to my cooool ui
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

//card slot id for holding cards in action slots
//-1 card = your action slot is empty
//-1 target = the enemy action slot is empty
//duh
#include <SFML/Graphics.hpp>
#include <cmath>

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

//hi vivek =.)
struct Slot {
    sf::Vector2f pos;
    int card = -1;
    int target = -1;
};

//determining card slot size (80x80 square u can adapt it)
static const sf::Vector2f kSlotSize(80.f, 80.f); //every slot uses this : )

//centering the text onto a still point for cards
//so its in the middle of the card
//CHANGE
//adapt this when card art/desc is done
static void centerText(sf::Text& t, sf::Vector2f center) {
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
    t.setPosition(center);
}

//loops over the slots and builds the rectangle around each one
//you better be actually reading my comments
//im spending a lot of time on these
//basically this is how it chooses what card ur hovering
static int slotAt(const std::vector<Slot>& slots, sf::Vector2f p) {
    for (int i = 0; i < static_cast<int>(slots.size()); ++i) {
        if (sf::FloatRect(slots[i].pos, kSlotSize).contains(p)) return i;
    }
    return -1;
}

//draws the. health bar. obv
//i feel like i wrote my code pretty clearly
//like is all this necessary
static void drawHealthBar(sf::RenderWindow& window, const sf::Font& font,
                          sf::Vector2f topLeft, float width, int hp, float shownHp, int maxHp) {
    const float height = 22.f;

    //creates a black rectangle that is black
    //as the backing for the health bar
    sf::RectangleShape back({width, height});
    back.setPosition(topLeft);
    back.setFillColor(sf::Color(60, 60, 60));
    back.setOutlineThickness(2.f);
    back.setOutlineColor(sf::Color::Black);
    window.draw(back);

    //red ghost bar = the hp you just lost (gradual drain)
    //shownHp chases the real hp
    //(see the animate section in the loop)
    sf::RectangleShape trail({width * (shownHp / static_cast<float>(maxHp)), height});
    trail.setPosition(topLeft);
    trail.setFillColor(sf::Color(230, 80, 80));
    window.draw(trail);

    //ratio = % of the bar i fill with color
    //represents % hp of total hp
    //call again when hp changes
    float ratio = static_cast<float>(hp) / static_cast<float>(maxHp);
    sf::RectangleShape fill({width * ratio, height});
    fill.setPosition(topLeft);
    fill.setFillColor(sf::Color(90, 200, 90));
    window.draw(fill);

    //puts the label on the health bar showing health
    //eg. '10/30hp'
    //and colors and centers it
    sf::Text label(font,  std::to_string(hp) + "/" + std::to_string(maxHp)+ "hp", 16);
    label.setFillColor(sf::Color::White);
    label.setOutlineColor(sf::Color::Black);
    label.setOutlineThickness(2.f);
    centerText(label, {topLeft.x + width / 2.f, topLeft.y + height / 2.f - 2.f});
    window.draw(label);
}

//i played deadlock today
//didnt really understand it and it took forever
//wonder why marvel rivals fell off?

//it draws the placeholder for the character
//CHANGE
//remove this when the actual drawings for the character are made and sub those in
static void drawCharacter(sf::RenderWindow& window, const sf::Font& font,
                          sf::Vector2f topLeft, sf::Color colour,
                          const std::string& name, int hp, float shownHp, int maxHp) {
    const sf::Vector2f size(140.f, 220.f);
    sf::RectangleShape box(size);
    box.setPosition(topLeft);
    box.setFillColor(colour);
    box.setOutlineThickness(3.f);
    box.setOutlineColor(sf::Color::Black);
    window.draw(box); //CHANGE^ this when sprites are made

    //draws the name of the character under it
    //remove if u dont want names idk
    //id adapt the colors of the name to the characters as wel
    sf::Text nameText(font, name, 22);
    nameText.setFillColor(sf::Color::White);
    nameText.setOutlineColor(sf::Color::Black);
    nameText.setOutlineThickness(3.f);
    centerText(nameText, {topLeft.x + size.x / 2.f, topLeft.y + size.y + 22.f});
    window.draw(nameText);

    //draws the health bar
    //makes the heath bar a litte wider than the character asw
    //keep this part i like it
    drawHealthBar(window, font, {topLeft.x - 10.f, topLeft.y + size.y + 42.f}, size.x + 20.f, hp, shownHp, maxHp);
}

//is it pingu or pengu?
//noot noot!

//draws an action slot
static void drawSlot(sf::RenderWindow& window, const sf::Font& font, const Slot& slot,
                     sf::Color base, bool selected, bool highlighted) {
    sf::RectangleShape box(kSlotSize);
    box.setPosition(slot.pos);

    //if the slot is filled, the color is solid and shows its full
    bool filled = slot.card != -1;
    sf::Color fill = base;
    //if the slot is not filled, the color is transparent and shows its empy
    if (!filled) fill.a = 110;
    box.setFillColor(fill);

    //hilights the selected slot yellow if you click it otherwise black
    //id CHANGE the color at some point
    if (selected || highlighted) {
        box.setOutlineThickness(5.f);
        box.setOutlineColor(sf::Color(255, 200, 40));
    } else {
        box.setOutlineThickness(3.f);
        box.setOutlineColor(filled ? sf::Color::Black : base);
    }
    window.draw(box);

    //temp to show which cards in each action slot
    //when sprites are available id CHANGE it to have a square image of the
    //card sprite in the action slot
    if (filled) {
        sf::Text label(font, "Card " + std::to_string(slot.card), 16);
        label.setFillColor(sf::Color::White);
        label.setOutlineColor(sf::Color::Black);
        label.setOutlineThickness(2.f);
        centerText(label, slot.pos + kSlotSize / 2.f);
        window.draw(label);
    }
}

//draws an arced arrow above everything so
//u can see what each things connected to
static void drawArcArrow(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end, sf::Color color) {
    const float headLen = 22.f;
    const float headHalfWidth = 12.f;
    const float thickness = 5.f;
    const float outline = 2.f; //outlined cuz it was kinda hard to read on bg CHANGE later
    const int segments = 48;

    //making sure the midway point is above everything so it doesnt arc into it
    //and vector direction of the arrow
    //yes i tested it.
    sf::Vector2f ctrl((start.x + end.x) / 2.f, std::min(start.y, end.y) - 160.f);
    sf::Vector2f d = end - ctrl;
    float len = std::sqrt(d.x * d.x + d.y * d.y);
    d /= len;
    //line ends at arrowhead
    sf::Vector2f lineEnd = end - d * headLen;

    //lowkey did google this one thanks lloyd
    //quadratic bezier curve for a smooth curve of the arrow over everything
    //yes this is the most efficient way sorry
    //takes 49 sample points adapt if not smooth enough
    std::vector<sf::Vector2f> pts;
    for (int i = 0; i <= segments; ++i) {
        float t = static_cast<float>(i) / segments;
        float u = 1.f - t;
        pts.push_back(start * (u * u) + ctrl * (2.f * u * t) + lineEnd * (t * t)); //curve formula
    }

    //oh my godddd im bored
    //blablabla we want colored arrows and outlines!
    //mason YOU work out how to do it
    //one day im going to Snap.

    //basically lines are not allowed to be thick in sfml so u have to do this
    //uses strips to thicken it out like a ribbon
    auto drawStrip = [&](float width, sf::Color c) {
        sf::VertexArray strip(sf::PrimitiveType::TriangleStrip, pts.size() * 2);
        for (std::size_t i = 0; i < pts.size(); ++i) {
            sf::Vector2f a = pts[i == 0 ? 0 : i - 1];
            sf::Vector2f b = pts[std::min(i + 1, pts.size() - 1)];
            sf::Vector2f dir = b - a;
            float l = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            dir /= l;
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

    // draws the triangular arrowhead
    // CHANGE so outline doesnt cut into fill
    sf::Vector2f perp(-d.y, d.x);
    sf::ConvexShape head(3);
    head.setPoint(0, end);
    head.setPoint(1, lineEnd + perp * headHalfWidth);
    head.setPoint(2, lineEnd - perp * headHalfWidth);
    head.setFillColor(color);
    head.setOutlineThickness(outline);
    head.setOutlineColor(sf::Color(20, 20, 20));
    window.draw(head);
}





//main
int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Card Game UI");
    window.setFramerateLimit(60);
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Could not load font. Check the path in main.cpp\n";
        return 1;
    }
    sf::Texture caveTexture;
    if (!caveTexture.loadFromFile("assets/cave.png")) { //cave bg scaled to screen
        std::cerr << "Could not load assets/cave.png. Make sure it is copied next to the executable.\n";
        return 1;
    }
    caveTexture.setSmooth(true);
    sf::Sprite cave(caveTexture);
    cave.setScale({1280.f / static_cast<float>(caveTexture.getSize().x),
                   720.f / static_cast<float>(caveTexture.getSize().y)});

    //put a black rectangle behind the deck/energy and stuff so its more readable
    //CHANGE after to something better
    const float panelTop = 530.f;
    sf::RectangleShape bottomPanel({1280.f, 720.f - panelTop});
    bottomPanel.setPosition({0.f, panelTop});
    bottomPanel.setFillColor(sf::Color::Black);

    //CHANGE to real backend values later
    //isaac and vivek n diya this where u do it
    //backend unite!
    //sorry
    int energy = 4;
    int maxEnergy = 4;
    int turnNumber = 1;
    int playerHp = 80, playerMaxHp = 100;
    int enemyHp = 45, enemyMaxHp = 60;
    float playerShownHp = static_cast<float>(playerHp);  //ghost bars start full
    float enemyShownHp = static_cast<float>(enemyHp);

    // character positions
    const sf::Vector2f playerPos(180.f, 240.f);
    const sf::Vector2f enemyPos(960.f, 240.f);
    const sf::Color blue(70, 120, 220);
    const sf::Color red(220, 70, 70);

    // calls 3 action slots above each char
    const float slotY = 110.f;
    const float slotGap = 20.f;
    auto makeSlots = [&](float centerX) {
        std::vector<Slot> slots(3);
        float totalWidth = 3.f * kSlotSize.x + 2.f * slotGap;
        for (int i = 0; i < 3; ++i) {
            slots[i].pos = {centerX - totalWidth / 2.f + i * (kSlotSize.x + slotGap), slotY};
        }
        return slots;
    };
    std::vector<Slot> playerSlots = makeSlots(playerPos.x + 70.f);
    std::vector<Slot> enemySlots = makeSlots(enemyPos.x + 70.f);
    int selectedCard = -1;  //card thats selected (-1=none)
    int selectedSlot = -1;  //slot thats selected (-1=none)

    //determines the color of the arrows drawn from your action slot to theirs
    //1st arrow = yellow, 2nd = red, 3rd = orange
    //so you can tell the difference between where your arrows are landing
    std::vector<int> arrowOrder;
    const sf::Color arrowColors[3] = {
        sf::Color(255, 230, 120),  // yellow
        sf::Color(235, 50, 50),    // red
        sf::Color(255, 150, 30),   // orange
    };

    //your hand (card numbers)
    //CHANGE - placeholders 1-6
    std::vector<int> hand = {1, 2, 3, 4, 5, 6};
    const sf::Vector2f cardSize(130.f, 180.f);
    const float cardStep = 90.f;   //distance between card left edges (reduce for more overlap)
    const float cardLift = 40.f;   //how far hovered card rises
    const float handY = 720.f - cardSize.y - 10.f;

    auto handStartX = [&]() {
        float w = cardStep * (static_cast<float>(hand.size()) - 1.f) + cardSize.x;
        return (1280.f - w) / 2.f;
    };

    //checks which cards under the point (right to left)
    //to determine which card clicked
    auto cardAt = [&](sf::Vector2f p) {
        for (int i = static_cast<int>(hand.size()) - 1; i >= 0; --i) {
            sf::FloatRect r({handStartX() + i * cardStep, handY - cardLift}, {cardSize.x, cardSize.y + cardLift});
            if (r.contains(p)) return i;
        }
        return -1;
    };

    //begin turn button
    sf::RectangleShape turnButton({280.f, 70.f});
    turnButton.setPosition({960.f, 610.f});
    turnButton.setOutlineThickness(3.f);
    turnButton.setOutlineColor(sf::Color::Black);

    //energy circle
    sf::CircleShape energyCircle(70.f);
    energyCircle.setPosition({40.f, 540.f});
    energyCircle.setFillColor(sf::Color(255, 244, 163));  // pastel yellow
    energyCircle.setOutlineThickness(4.f);
    energyCircle.setOutlineColor(sf::Color(30, 30, 30));

    //card lift animation
    //each card (by id) remembers the raise height its @
    //every frame it moves a fraction of the way to where it should be
    std::map<int, float> liftAmount;
    const float cardAnimSpeed = 18.f;  //increase num for faster //duh
    sf::Clock clock;

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
                if (key->code == sf::Keyboard::Key::D) enemyHp = std::max(0, enemyHp - 10);
                if (key->code == sf::Keyboard::Key::A) playerHp = std::max(0, playerHp - 10);
                if (key->code == sf::Keyboard::Key::H) {
                    enemyHp = enemyMaxHp;
                    playerHp = playerMaxHp;
                }
            }
            if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f m = window.mapPixelToCoords(click->position);
                if (click->button == sf::Mouse::Button::Left) {
                    if (turnButton.getGlobalBounds().contains(m)) {
                        turnNumber++;  // CHANGE placeholder later this will run the queued actions
                    } else {
                        int c = cardAt(m);
                        int ps = slotAt(playerSlots, m);
                        int es = slotAt(enemySlots, m);

                        if (c != -1) {
                            //clicking a card selects it (right click for deselect)
                            selectedCard = (c == selectedCard) ? -1 : c;
                            selectedSlot = -1;
                        } else if (ps != -1) {
                            if (selectedCard != -1 && playerSlots[ps].card == -1) {
                                //places the card into a box if it is empty
                                playerSlots[ps].card = hand[selectedCard];
                                hand.erase(hand.begin() + selectedCard);
                                selectedCard = -1;
                                selectedSlot = -1;
                            } else if (playerSlots[ps].card != -1) {
                                //selects a clicked filled box so u can attack enemy box
                                selectedSlot = (ps == selectedSlot) ? -1 : ps;
                                selectedCard = -1;
                            }
                        } else if (es != -1) {
                            //clicking an enemy slot selects it if ur holding a filled action box
                            if (selectedSlot != -1 && playerSlots[selectedSlot].card != -1) {
                                playerSlots[selectedSlot].target = es;
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
                    if (ps != -1 && playerSlots[ps].card != -1) {
                        hand.push_back(playerSlots[ps].card);
                        std::sort(hand.begin(), hand.end());
                        playerSlots[ps].card = -1;
                        playerSlots[ps].target = -1;
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
        bool targeting = selectedSlot != -1 && playerSlots[selectedSlot].card != -1;
        int hoveredEnemySlot = targeting ? slotAt(enemySlots, mouse) : -1;
        int hoveredPlayerSlot = -1;  //hilight empty blue boxes while a card is selected
        if (selectedCard != -1) {
            int h = slotAt(playerSlots, mouse);
            if (h != -1 && playerSlots[h].card == -1) hoveredPlayerSlot = h;
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

        //animate health bar drain
        //green bar drops instantly, red ghost bar eases down behind it
        auto drain = [&](float& shown, int hp) {
            if (shown < hp) shown = static_cast<float>(hp);       //healing: ghost snaps up
            shown += (hp - shown) * (1.f - std::exp(-3.f * dt));  //damage: ghost eases down (3 = speed)
            if (std::abs(shown - hp) < 0.05f) shown = static_cast<float>(hp);
        };
        drain(playerShownHp, playerHp);
        drain(enemyShownHp, enemyHp);

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

        //drawing characters
        drawCharacter(window, font, playerPos, blue, "Player", playerHp, playerShownHp, playerMaxHp);
        drawCharacter(window, font, enemyPos, red, "Enemy", enemyHp, enemyShownHp, enemyMaxHp);

        //action slots
        for (int i = 0; i < 3; ++i) {
            drawSlot(window, font, playerSlots[i], blue, i == selectedSlot, i == hoveredPlayerSlot);
            drawSlot(window, font, enemySlots[i], red, false, i == hoveredEnemySlot);
        }

        //black area bottom of screen
        window.draw(bottomPanel);

        //your hand
        //show order normal cards < selected card < hovered card
        float startX = handStartX();
        for (int pass = 0; pass < 3; ++pass) {
            for (int i = 0; i < static_cast<int>(hand.size()); ++i) {
                bool isHovered = (i == hoveredCard);
                bool isSelected = (i == selectedCard);
                int priority = isHovered ? 2 : (isSelected ? 1 : 0);
                if (priority != pass) continue;

                bool raised = isHovered || isSelected;
                float y = handY - liftAmount[hand[i]];  //animated height instead of a hard jump

                //card grows up to 10% bigger as it rises
                //grows upwards so the bottom edge stays put
                float t = liftAmount[hand[i]] / cardLift;  //0 = resting, 1 = fully raised
                float scale = 1.f + 0.1f * t;
                sf::Vector2f size = cardSize * scale;
                float top = y - (size.y - cardSize.y);
                //rounded corners (size, corner radius, points per corner)
                RoundedRectangle card(size, 5.f, 8);
                card.setPosition({startX + i * cardStep - (size.x - cardSize.x) / 2.f, top});
                if (isSelected) {
                    card.setFillColor(sf::Color(205, 203, 190));  // selected cards are tinted
                } else {
                    //washed out gray, a bit lighter when hovered
                    card.setFillColor(isHovered ? sf::Color(195, 195, 200) : sf::Color(168, 168, 174));
                }
                card.setOutlineThickness(isSelected ? 6.f : (isHovered ? 5.f : 3.f));
                card.setOutlineColor(raised ? sf::Color(255, 200, 40) : sf::Color(30, 30, 30));
                window.draw(card);

                sf::Text label(font, "Card " + std::to_string(hand[i]), 20);
                label.setFillColor(sf::Color::Black);
                centerText(label, {startX + i * cardStep + cardSize.x / 2.f, top + 25.f});
                window.draw(label);
            }
        }

        //energy circle
        window.draw(energyCircle);
        sf::Text energyText(font, std::to_string(energy) + "/" + std::to_string(maxEnergy), 48);
        energyText.setFillColor(sf::Color::Black);
        centerText(energyText, {40.f + 70.f, 540.f + 70.f});
        window.draw(energyText);

        //begin turn button
        turnButton.setFillColor(overButton ? sf::Color(255, 255, 255) : sf::Color(235, 235, 235));
        window.draw(turnButton);
        sf::Text turnText(font, "Begin Turn " + std::to_string(turnNumber), 30);
        turnText.setFillColor(sf::Color::Black);
        centerText(turnText, {960.f + 140.f, 610.f + 35.f});
        window.draw(turnText);

        //arrows in drawn order (yellow, red, orange)
        for (std::size_t k = 0; k < arrowOrder.size(); ++k) {
            const Slot& s = playerSlots[arrowOrder[k]];
            if (s.card != -1 && s.target != -1) {
                sf::Vector2f from(s.pos.x + kSlotSize.x / 2.f, s.pos.y - 2.f);
                sf::Vector2f to(enemySlots[s.target].pos.x + kSlotSize.x / 2.f,
                                enemySlots[s.target].pos.y - 4.f);
                drawArcArrow(window, from, to, arrowColors[k % 3]);
            }
        }
        window.display();
    }
    return 0;
}
