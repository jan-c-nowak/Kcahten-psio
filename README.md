# Kcahten-psio
Simple 2D Eagle-view Action RPG game

Polski:
Aby skompilować grę należy ustawić najpierw tryb w VS na x86 i debug. Jest możliwość uruchomienia poprzez plik exe w folderze Debug.
Projekt należy najpierw wypakować.

Sterowanie to WSAD + Spacja (Poruszanie + Atak). Pierwszy atak wykonany zostanie po kilku sekundach, dalej działa normalnie.

W przypadku uruchamiania przez Visual Studio:
Gdyby visual studio nie odnalazł ścieżek do grafik, to należy zmienić tylko ścieżki w TextureManager.hpp, ewentualnie można zmienić ścieżkę fontu w Game.cpp.

Nie zajmuje się grafiką oraz game designem, więc na te cechy gry patrz z przymrużeniem oka.

Gra została stworzona jako projekt zaliczeniowy na zajęcia "Programowanie Obiektowe", w którym brałem udział podczas semestru letniego na Politechnice Poznańskiej.

baw się dobrze :),

jan nowak

==================================================================================================================

English:

In order to compile the code, you'll need to set Visual Studio project settings to x86 and debug (both options avaliable in the top toolbar). It is possible to open the game using the .exe file from repo; it is inside the 'Debug' folder. You might need to unpack the folder first.

Use WASD to move and SPACE to attack. There is a minor bug, where your first attack will be executed after a while, the next ones work properly.

In case you compile the program with VS, be aware that you might need to change the paths to the graphics files. You can change that easily within the TextureManager.hpp file. The font path can be changed in Game.cpp

I'm not a graphics person nor a game designer, so take the looks and playability of the game with a pinch of salt. It's mostly a demo of my Cpp/SFML/OOP skills done sometime in the September of 2021.

The game was initially my final project for "The Introduction to OOP" course i took during the summer semester at Poznań University of Technology.

hope you have fun :),

jan nowak
