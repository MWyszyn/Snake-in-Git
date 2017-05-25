#include <SFML/Graphics.hpp>

#include <vector>
#include <utility>
#include <array>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>

namespace
{
    const sf::Vector2f WindowSize(640, 480);
    constexpr auto BlockSize = 16.f;
    constexpr auto SnakeSpeed = 5.f;
    const auto TimeStep = sf::seconds(1 / SnakeSpeed);

    auto randomEngine()
    {
        std::array<std::mt19937::result_type, std::mt19937::state_size> seed_data;
        thread_local std::random_device source;
        std::generate(std::begin(seed_data), std::end(seed_data), std::ref(source));
        std::seed_seq seeds(std::begin(seed_data), std::end(seed_data));
        thread_local std::mt19937 seeded_engine(seeds);
        return seeded_engine;
    }

    auto random(const std::uniform_real_distribution<float>& dist)
    {
        static auto& RandomEngine = randomEngine();
        return dist(RandomEngine);
    }
}

class Snake : public sf::Drawable, sf::NonCopyable
{
public:
    enum Direction
    {
        None,
        Up,
        Down,
        Left,
        Right
    };

private:
    using SnakeContainer = std::vector<sf::RectangleShape>;


public:
    explicit Snake(const sf::Font& font)
    {
        reset();

        mLivesText.setFont(font);
        mLivesText.setStyle(sf::Text::Bold);
        mLivesText.setCharacterSize(30);
        mLivesText.setColor(sf::Color::White);
        mLivesText.setPosition(WindowSize.x - 160.f, 0.f);
    }

    void reset()
    {
        mSnakeBody.clear();
        sf::RectangleShape shape({ BlockSize - 1, BlockSize - 1 });

        shape.setPosition(70 + BlockSize, 70 + 3 * BlockSize);
        shape.setFillColor(sf::Color(211, 211, 211));
        mSnakeBody.push_back(shape);

        shape.setPosition(70 + BlockSize, 70 + 2 * BlockSize);
        mSnakeBody.push_back(shape);

        shape.setPosition(70 + BlockSize, 70 + BlockSize);
        mSnakeBody.push_back(shape);

        mDirection = Direction::None;
        mLives = 3;
        mLivesText.setString("Lives: " + std::to_string(mLives));
    }

    void setDirection(Direction dir)
    {
        if (mDirection == Direction::None && dir == Direction::Up)
            return;

        if (mDirection == Direction::Up && dir == Direction::Down)
            return;

        if (mDirection == Direction::Down && dir == Direction::Up)
            return;

        if (mDirection == Direction::Left && dir == Direction::Right)
            return;

        if (mDirection == Direction::Right && dir == Direction::Left)
            return;

        mDirection = dir;
    }

    bool hasLost() const
    {
        return mLives <= 0;
    }

    void lose()
    {
        mLives = 0;
    }

    void extend()
    {
        sf::RectangleShape shape({ BlockSize - 1, BlockSize - 1 });
        const auto& tail = mSnakeBody[mSnakeBody.size() - 1];

        if (mDirection == Direction::Up)
            shape.setPosition(tail.getPosition().x, tail.getPosition().y + BlockSize);

        else if (mDirection == Direction::Down)
            shape.setPosition(tail.getPosition().x, tail.getPosition().y - BlockSize);

        else if (mDirection == Direction::Left)
            shape.setPosition(tail.getPosition().x + BlockSize, tail.getPosition().y);

        else if (mDirection == Direction::Right)
            shape.setPosition(tail.getPosition().x - BlockSize, tail.getPosition().y);

        shape.setFillColor(sf::Color(211, 211, 211));
        mSnakeBody.push_back(shape);
    }

    void update()
    {
        if (mDirection == Direction::None)
            return;

        move();
        checkCollision();
    }

    sf::FloatRect getGlobalBounds() const
    {
        return mSnakeBody.front().getGlobalBounds();
    }

    bool hasCollideWithFruit(const sf::CircleShape& shape) const
    {
        for (const auto& part : mSnakeBody)
        {
            if (part.getGlobalBounds().intersects(shape.getGlobalBounds()))
                return true;
        }

        return false;
    }


private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        for (const auto& part : mSnakeBody)
            target.draw(part, states);

        target.draw(mLivesText, states);
    }

    void checkCollision()
    {
        if (mSnakeBody.size() < 5)
            return;

        const auto& head = mSnakeBody.front();

        bool is_first_iteration = true;

        for (const auto& part : mSnakeBody)
        {
            if (std::exchange(is_first_iteration, false)) continue;

            if (part.getPosition() == head.getPosition())
            {
                int segments = &*(mSnakeBody.end() - 1) - &part;
                shrink(segments);
                break;
            }
        }
    }

    void move()
    {
        std::copy_backward(mSnakeBody.begin(), mSnakeBody.end() - 1, mSnakeBody.end());

        if (mDirection == Direction::Left)
            mSnakeBody.front().move(-BlockSize, 0);

        else if (mDirection == Direction::Right)
            mSnakeBody.front().move(BlockSize, 0);

        else if (mDirection == Direction::Up)
            mSnakeBody.front().move(0, -BlockSize);

        else if (mDirection == Direction::Down)
            mSnakeBody.front().move(0, BlockSize);
    }

    void shrink(int segments)
    {
        for (auto i = 0; i < segments; ++i)
            mSnakeBody.pop_back();

        --mLives;

        mLivesText.setString("Lives: " + std::to_string(mLives));
    }


private:
    SnakeContainer mSnakeBody;
    Direction mDirection;
    std::size_t mLives;
    sf::Text mLivesText;
};

class World : public sf::Drawable, sf::NonCopyable
{
    using BordersContainer = std::vector<sf::RectangleShape>;
    using DistType = std::uniform_real_distribution<float>;


public:
    explicit World(const sf::Font& font)
        : mSnake(font)
        , mBorders(4)
        , mDistX(2 * BlockSize, WindowSize.x - 2 * BlockSize)
        , mDistY(3 * BlockSize, WindowSize.y - 2 * BlockSize)
        , mScore()
    {
        mFruit.setFillColor(sf::Color(211, 211, 211));
        mFruit.setRadius(BlockSize / 2.f);
        respawnFruit();

        // initial Wall borders
        for (auto& side : mBorders)
        {
            auto index = &side - &*mBorders.begin();

            if (index % 2 == 0)
                side.setSize({ WindowSize.x, BlockSize });
            else
                side.setSize({ BlockSize, WindowSize.y });

            if (index == 1)
                side.setPosition(WindowSize.x - BlockSize, 2 * BlockSize);
            else if (index == 2)
                side.setPosition(0, WindowSize.y - BlockSize);
            else
                side.setPosition(0, 2 * BlockSize);

            side.setFillColor(sf::Color(211, 211, 211));
        }

        mScoreText.setString("Score: " + std::to_string(mScore));
        mScoreText.setFont(font);
        mScoreText.setStyle(sf::Text::Bold);
        mScoreText.setCharacterSize(30);
        mScoreText.setColor(sf::Color::White);
        mScoreText.setPosition(30.f, 0.f);
    }

    void update(sf::Time& dt)
    {
        // real time input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            mSnake.setDirection(Snake::Up);

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            mSnake.setDirection(Snake::Down);

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            mSnake.setDirection(Snake::Left);

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            mSnake.setDirection(Snake::Right);

        if (mSnake.hasLost())
            mSnake.reset();

        if (mFruit.getGlobalBounds().intersects(mSnake.getGlobalBounds()))
        {
            mSnake.extend();
            mScoreText.setString("Score: " + std::to_string(mScore += 10));
            respawnFruit();
        }

        for (const auto& side : mBorders)
        {
            if (side.getGlobalBounds().intersects(mSnake.getGlobalBounds()))
            {
                mScore = 0;
                mScoreText.setString("Score: " + std::to_string(mScore));
                mSnake.lose();
            }
        }

        if (dt < TimeStep) return;

        dt -= TimeStep;

        mSnake.update();
    }


private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        for (const auto& side : mBorders)
            target.draw(side, states);

        target.draw(mFruit, states);
        target.draw(mSnake, states);
        target.draw(mScoreText, states);
    }

    void respawnFruit()
    {
        do
            mFruit.setPosition(random(mDistX), random(mDistY));
        while (mSnake.hasCollideWithFruit(mFruit));
    }


private:
    Snake mSnake;
    sf::CircleShape mFruit;
    std::size_t mScore;
    sf::Text mScoreText;
    BordersContainer mBorders;
    DistType mDistX;
    DistType mDistY;
};

class Game : sf::NonCopyable
{
public:
    Game()
        : mWindow(sf::VideoMode(static_cast<int>(WindowSize.x), static_cast<int>(WindowSize.y)), "Snake")
        , mWorld(mFont)
    {
        if (!mFont.loadFromFile("arial.ttf"))
        {
            throw "Can't load font file";
        }
    }

    void run()
    {
        sf::Clock clock;
        sf::Time timeElapsed = sf::Time::Zero;

        while (mWindow.isOpen())
        {
            timeElapsed += clock.restart();
            processEvents();
            update(timeElapsed);
            render();
        }
    }


private:
    void processEvents()
    {
        sf::Event event;

        while (mWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                mWindow.close();
        }
    }

    void update(sf::Time& dt)
    {
        mWorld.update(dt);
    }

    void render()
    {
        mWindow.clear();
        mWindow.draw(mWorld);
        mWindow.display();
    }


private:
    sf::RenderWindow mWindow;
    World mWorld;
    sf::Font mFont;
};

int main()
{
    try
    {
        Game game;
        game.run();
    }
    catch (std::runtime_error& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
        return 1;
    }
}
