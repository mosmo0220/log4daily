**milestonesComponent**

Plik: milestonesComponent.cpp

Opis: Funkcja `milestonesComponent` tworzy komponent "Milestones", który wyświetla listę kamieni milowych. 

* Zmienne globalne:
    * `milestones`: Wektor nazw kamieni milowych.
    * `selected_milestones`: Indeks aktualnie wybranego kamienia milowego.

* Elementy interfejsu:
    * Lista kamieni milowych z możliwością wyboru.

**diaryComponent**

Plik: diaryComponent.cpp

Opis: Funkcja `diaryComponent` tworzy komponent "Diary", który wyświetla wpisy do dziennika. 

* Zmienne globalne:
    * `diary`: Wektor wpisów do dziennika.
    * `selected_diary`: Indeks aktualnie wybranego wpisu.

* Elementy interfejsu:
    * Lista wpisów do dziennika z możliwością wyboru.

**aboutComponent**

Plik: aboutComponent.cpp

Opis: Funkcja `aboutComponent` tworzy komponent "About", który wyświetla informacje o aplikacji. 

* Zmienne globalne:
    * `about`: Wektor informacji o aplikacji.
    * `selected_about`: Indeks aktualnie wybranej informacji.

* Elementy interfejsu:
    * Lista informacji o aplikacji z możliwością wyboru.

**createComponent**

Plik: createComponent.cpp

Opis: Funkcja `CreateComponent` tworzy generyczny komponent UI z listą elementów i etykietą. 

* Argumenty:
    * `entries`: Wektor ciągów znaków reprezentujących elementy listy.
    * `selected`: Indeks aktualnie wybranego elementu.
    * `content`: Etykieta komponentu.

* Elementy interfejsu:
    * Lista elementów z możliwością wyboru.
    * Etykieta komponentu.