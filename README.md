# Tetris 3D

3D-версияигры Тетрис, написанная на C++ с использованием OpenGL и GLUT.

## Особенности игры

- Полноценная 3D-реализация механик Тетриса
- Вращение фигур по трём осям (X, Y, Z)
- Два режима отображения: вид сверху и сбоку(клавиши 1 или 0)
- Изначально игра стоит на паузе, чтобы ее запусить необходимо нажать кнопку S

## Быстрый старт

### Клонирование репозитория
```bash
git clone https://github.com/saslekawaiii/3d-tetris.git
cd 3d-tetris
 ```
### Сборка приложения MacOS
```bash
clang++ -std=c++17 -o Tetris3D mainer.cpp shape.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -framework OpenGL -framework GLUT
```
