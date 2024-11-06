**MainThread**

Plik: mainThread.cpp

Opis: Klasa `MainThread` zarządza głównym wątkiem aplikacji log4daily. Obsługuje parsowanie danych wejściowych z konsoli, filtrowanie obsługiwanych komend i ich wykonywanie.

* Metody publiczne:
    * `MainThread(std::string workingDirectory, std::string configPath)`: Konstruktor, który inicjalizuje `MainThread` z określonym katalogiem roboczym i ścieżką do pliku konfiguracyjnego.
    * `CommandType run(int argc, char* argv[])`: Parsuje i wykonuje komendy z argumentów wiersza poleceń.
    * `void showMessage(CommandType respond, std::string message = "")`: Wyświetla komunikat w konsoli na podstawie wyniku wykonania komendy.
    * `std::string getRespondMessage()`: Zwraca komunikat odpowiedzi po wykonaniu komendy.
    * `FileData getOpenedFile()`: Zwraca dane otwartego pliku log4daily.

* Metody prywatne:
    * `void showHelp()`: Wyświetla pomoc dla dostępnych komend.

* Atrybuty:
    * `localStorage`: Obiekt `LocalStorage` do zarządzania plikami log4daily.
    * `workingDirectory`: Katalog roboczy aplikacji.
    * `configName`: Nazwa pliku konfiguracyjnego.
    * `openedFile`: Dane otwartego pliku log4daily.
    * `respondMessage`: Komunikat odpowiedzi po wykonaniu komendy.

* Typ wyliczeniowy `CommandType`:
    * Definiuje typy komend, które mogą być wykonane przez aplikację: `New`, `Open`, `Delete`, `Help`, `Unsupported`, `Failed`, `Other`.

