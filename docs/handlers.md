**handlers**

Plik: handlers.cpp

Opis: Przestrzenie nazw `consoleHandlers` i `controlHandlers` zawierają funkcje do parsowania i filtrowania komend z konsoli.

* `consoleHandlers`:
    * `struct Command`: Struktura reprezentująca komendę z nazwą i argumentem.
    * `std::vector<Command> parseConsoleInputs(int argc, char* argv[])`: Parsuje argumenty wiersza poleceń do wektora struktur `Command`.

* `controlHandlers`:
    * `static const std::vector<std::string> supportedCommands`: Wektor obsługiwanych komend.
    * `std::vector<consoleHandlers::Command> filterForSupportedCommands(const std::vector<consoleHandlers::Command>& commands)`: Filtruje wektor komend, zwracając tylko obsługiwane komendy.