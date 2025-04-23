#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Окно
    sf::RenderWindow window(
        sf::VideoMode(1000, 700),
        "Button on SFML 2.6",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    // Фон
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("background.png")) {
        std::cerr << "Error: can't load background.png\n";
        return 1;
    }
    sf::Sprite bg(bgTexture);
    bg.setScale(
        float(window.getSize().x) / float(bgTexture.getSize().x),
        float(window.getSize().y) / float(bgTexture.getSize().y)
    );

    // Поле ввода
    sf::RectangleShape inputBox(sf::Vector2f(400.f, 50.f));
    inputBox.setFillColor(sf::Color(255, 255, 255, 200));
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(2.f);
    inputBox.setPosition(300.f, 100.f);

    // Кнопка
    sf::RectangleShape button(sf::Vector2f(200.f, 65.f));
    button.setFillColor(sf::Color(130, 70, 88));
    button.setOutlineColor(sf::Color::White);
    button.setOutlineThickness(2.f);
    button.setPosition(400.f, 600.f);

    // Шрифт
    sf::Font font;
    if (!font.loadFromFile("XI20.ttf")) {
        std::cerr << "Font error: XI20.ttf not found\n";
        return 1;
    }

    // Тексты
    sf::String userInput;
    sf::Text inputText("", font, 30);
    inputText.setStyle(sf::Text::Bold);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(inputBox.getPosition() + sf::Vector2f(10.f, 10.f));

    sf::Text buttonLabel("Push me!", font, 36);
    buttonLabel.setStyle(sf::Text::Bold);
    {
        auto bounds = buttonLabel.getLocalBounds();
        buttonLabel.setOrigin(
            bounds.left + bounds.width / 2,
            bounds.top + bounds.height / 2
        );
        buttonLabel.setPosition(
            button.getPosition() + sf::Vector2f(button.getSize().x / 2, button.getSize().y / 2)
        );
    }

    // Главный цикл
    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            // Закрытие окна
            if (ev.type == sf::Event::Closed) {
                window.close();
                break;
            }

            // Ввод текста
            if (ev.type == sf::Event::TextEntered) {
                auto c = ev.text.unicode;
                if (c == 8) { // Backspace
                    if (!userInput.isEmpty())
                        userInput.erase(userInput.getSize() - 1, 1);
                }
                else if (c >= 32 && c != 127) {
                    const float padding = 10.f;
                    float maxW = inputBox.getSize().x - padding * 2;
                    sf::String tmp = userInput;
                    tmp += c;
                    inputText.setString(tmp);
                    if (inputText.getLocalBounds().width <= maxW)
                        userInput = tmp;
                }
                inputText.setString(userInput);
            }

            // Клик мышью
            if (ev.type == sf::Event::MouseButtonPressed) {
                if (ev.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos(ev.mouseButton.x, ev.mouseButton.y);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                    if (inputBox.getGlobalBounds().contains(worldPos)) {
                        userInput.clear();
                        inputText.setString("");
                    }
                    else if (button.getGlobalBounds().contains(worldPos)) {
                        sf::String greeting = L"Привет, " + userInput;
                        userInput = greeting;
                        inputText.setString(greeting);
                    }
                }
            }
        }

        // Рендер
        window.clear();
        window.draw(bg);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(button);
        window.draw(buttonLabel);
        window.display();
    }

    return 0;
}