#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp> 
#include <SFML/System.hpp>
#include <vector>
#include <iostream>

using namespace sf;

Vector2f start_of_field;

enum class state {
    CROSS, NOUGHT, EMPTY
};
struct Cell {
    RectangleShape cell;
    state status;
};

ConvexShape getCross() {
    ConvexShape cross;
    cross.setPointCount(12);
    cross.setPoint(0, Vector2f(0.f, 10.f));
    cross.setPoint(1, Vector2f(10.f, 0.f));
    cross.setPoint(2, Vector2f(120.f, 110.f)); //
    cross.setPoint(3, Vector2f(230.f, 0.f));
    cross.setPoint(4, Vector2f(240.f, 10.f));
    cross.setPoint(5, Vector2f(130.f, 120.f)); //
    cross.setPoint(6, Vector2f(240.f, 230.f));
    cross.setPoint(7, Vector2f(230.f, 240.f));
    cross.setPoint(8, Vector2f(120.f, 130.f)); // 
    cross.setPoint(9, Vector2f(10.f, 240.f));
    cross.setPoint(10, Vector2f(0.f, 230.f));
    cross.setPoint(11, Vector2f(110.f, 120.f)); //
    cross.setFillColor(Color(67, 75, 77));
    return cross;
}

CircleShape getNought() {
    CircleShape nought;
    nought.setRadius(110);
    nought.setOutlineThickness(10);
    nought.setFillColor(Color(50, 168, 145));
    nought.setOutlineColor(Color(190, 201, 204));
    nought.setPointCount(300);
    return nought;
}

int main() {
    start_of_field.x = (1920 - 900) / 2;
    start_of_field.y = (1080 - 900) / 2;
    RenderWindow window(sf::VideoMode(1920, 1080), "SFML window");

    Font font;
    if (!font.loadFromFile("Poppins-Medium.ttf"))
        return EXIT_FAILURE;

    std::vector<ConvexShape> crosses;
    std::vector<CircleShape> noughts;
    std::vector<RectangleShape> boundsY(4), boundsX(4);

    for (int i = 0; i < 4; i++) {
        boundsX[i].setPosition(start_of_field.x - 10, start_of_field.y);
        boundsX[i].setSize(Vector2f(880, 10));
        boundsX[i].setFillColor(Color(59, 93, 102));
        boundsY[i].setPosition(start_of_field.x, start_of_field.y - 10);
        boundsY[i].setSize(Vector2f(10, 880));
        boundsY[i].setFillColor(Color(59, 93, 102));

        boundsX[i].move(0, 300 * i - 10 * (i + 1));
        boundsY[i].move(300 * i - 10 * (i + 1), 0);
    }

    ConvexShape currentX;
    CircleShape currentO;

    int queue = 0; // 0 - X, 1 - O;
    bool isDraw = false, isSleep = true;
    state winner = state::EMPTY;

    Cell field[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            field[i][j].cell.setPosition(start_of_field);
            field[i][j].cell.setSize(Vector2f(280, 280));
            field[i][j].cell.setFillColor(Color(50, 168, 145));
            field[i][j].cell.move(300 * i - 10 * i, 300 * j - 10 * j);
            field[i][j].status = state::EMPTY;
        }
    }

    while (window.isOpen())
    {
        Vector2i mouse_pos = Mouse::getPosition(window);
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // turn
            if (event.type == Event::MouseButtonPressed) 
                if (event.key.code == Mouse::Left) 
                    for (int i = 0; i < 3; i++) 
                        for (int j = 0; j < 3; j++) 
                            if (field[i][j].cell.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y) && field[i][j].status == state::EMPTY) {
                                if (queue % 2) {
                                    currentO = getNought();
                                    currentO.setPosition(field[i][j].cell.getPosition());
                                    currentO.move(30, 30);
                                    noughts.push_back(currentO);
                                    field[i][j].status = state::NOUGHT;
                                    ++queue;
                                }
                                else {
                                    currentX = getCross();
                                    currentX.setPosition(field[i][j].cell.getPosition());
                                    currentX.move(20, 20);
                                    crosses.push_back(currentX);
                                    field[i][j].status = state::CROSS;
                                    ++queue;
                                }
                            }

            // restart
            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::BackSpace) {
                    crosses.clear();
                    noughts.clear();
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            field[i][j].status = state::EMPTY;
                        }
                    }
                    winner = state::EMPTY;
                    isSleep = true;
                    isDraw = false;
                    queue = 0;
                }
        }

        window.clear(Color(50, 168, 145));
        
        // the end
        if (winner != state::EMPTY || isDraw) {
            if (isSleep) {
                sleep(seconds(0.5));
            }
            Text text;
            text.setFont(font);
            text.setCharacterSize(100);
            if (winner == state::CROSS) {
                currentX.setPosition(660, 150);
                currentX.setScale(2.5, 2.5);
                window.draw(currentX);
                text.setString("WINNER!");
                text.setPosition((1920 - text.getGlobalBounds().width) / 2, 900);
                text.setFillColor(Color(190, 201, 204));
                window.draw(text);
            }
            if (winner == state::NOUGHT) {
                currentO.setPosition(660, 150);
                currentO.setScale(2.5, 2.5);
                window.draw(currentO);
                text.setString("WINNER!");
                text.setPosition((1920 - text.getGlobalBounds().width) / 2, 900);
                text.setFillColor(Color(67, 75, 77));
                window.draw(text);
            }
            if (winner == state::EMPTY) {
                text.setString("DRAW");
                text.setPosition((1920 - text.getGlobalBounds().width) / 2, (1080 - text.getGlobalBounds().height) / 2);
                window.draw(text);
            }
            isSleep = false;
        }
        // draw
        else { 
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    window.draw(field[i][j].cell);
                }
            }

            for (int i = 0; i < 4; i++) {
                window.draw(boundsX[i]);
                window.draw(boundsY[i]);
            }

            for (int i = 0; i < crosses.size(); i++) {
                window.draw(crosses[i]);
            }

            for (int i = 0; i < noughts.size(); i++) {
                window.draw(noughts[i]);
            }

            if (crosses.size() == 5 && noughts.size() == 4)
                isDraw = true;
        }
        window.display();

        if ((field[0][0].status == field[0][1].status) && (field[0][0].status == field[0][2].status) && (field[0][0].status != state::EMPTY)) // левая вертикаль
            winner = field[0][0].status; 
        if ((field[0][0].status == field[1][1].status) && (field[0][0].status == field[2][2].status) && (field[0][0].status != state::EMPTY)) // главная диагональ
            winner = field[0][0].status;
        if ((field[0][0].status == field[1][0].status) && (field[0][0].status == field[2][0].status) && (field[0][0].status != state::EMPTY)) // верхняя горизонталь 
            winner = field[0][0].status;
        if ((field[2][0].status == field[1][1].status) && (field[2][0].status == field[0][2].status) && (field[2][0].status != state::EMPTY)) // побочная диагональ
            winner = field[2][0].status;
        if ((field[2][2].status == field[1][2].status) && (field[2][2].status == field[0][2].status) && (field[2][2].status != state::EMPTY)) // нижняя горизонталь
            winner = field[2][2].status;
        if ((field[2][2].status == field[2][1].status) && (field[2][2].status == field[2][0].status) && (field[2][2].status != state::EMPTY)) // правая вертикаль
            winner = field[2][2].status;
        if ((field[0][1].status == field[1][1].status) && (field[0][1].status == field[2][1].status) && (field[0][1].status != state::EMPTY)) // средняя горизонталь
            winner = field[0][1].status;
        if ((field[1][0].status == field[1][1].status) && (field[1][0].status == field[1][2].status) && (field[1][0].status != state::EMPTY)) // средняя вертикаль
            winner = field[1][0].status;
    }

    return EXIT_SUCCESS;
}