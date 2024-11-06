**RenderUI**

Plik: renderUI.cpp

Opis: Klasa `RenderUI` odpowiada za renderowanie interfejsu użytkownika aplikacji log4daily.

* Metody publiczne:
    * `RenderUI(MainThread _mainThread)`: Konstruktor, który inicjalizuje `RenderUI` z obiektem `MainThread`.
    * `Component createExitComponent(ScreenInteractive& screen, int& exitSelected)`: Tworzy komponent wyjścia z aplikacji z opcjami zapisu i wyjścia bez zapisu.
    * `int renderUI()`: Renderuje komponenty UI i obsługuje interakcje użytkownika.
    * `std::string getExitMessage()`: Zwraca komunikat o wyjściu z aplikacji (z lub bez zapisu).

* Atrybuty:
    * `mainThread`: Obiekt `MainThread` do zarządzania głównym wątkiem aplikacji.
    * `inUseFileData`: Dane pliku log4daily używane w UI.
    * `exitedWithoutSaving`: Flaga wskazująca, czy użytkownik wyszedł z aplikacji bez zapisywania zmian.