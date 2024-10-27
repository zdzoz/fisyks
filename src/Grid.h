#pragma once

#include <optional>
#include <vector>

template <typename T>
class Grid {
public:
    Grid(int width, int height)
        : _width(width)
        , _height(height)
    {
        _grid.resize(_width * _height);
    }

    Grid(int width, int height, T&& value)
        : _width(width)
        , _height(height)
    {
        _grid.resize(_width * _height);
        setAll(value);
    }

    inline int width() const
    {
        return _width;
    }

    inline int height() const
    {
        return _height;
    }

    void setAll(const T& val)
    {
        for (auto& e : _grid) {
            e = val;
        }
    }

    std::optional<std::reference_wrapper<const T>> get(int x, int y) const
    {
        // TODO: restrict to debug mode
        if (x >= width() || x < 0 || y >= height() || y < 0)
            return std::nullopt;

        return _grid[y * width() + x];
    }

    bool set(const T& val, int x, int y)
    {
        // TODO: restrict to debug mode
        if (x >= width() || x < 0 || y >= height() || y < 0)
            return false;

        _grid[y * width() + x] = val;
        return true;
    }

    bool set(T&& val, int x, int y)
    {
        // TODO: restrict to debug mode
        if (x >= width() || x < 0 || y >= height() || y < 0)
            return false;

        _grid[y * width() + x] = std::move(val);
        return true;
    }

private:
    int _width;
    int _height;
    std::vector<T> _grid;
};
