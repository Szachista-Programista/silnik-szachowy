# Silnik Szachowy

Witaj! Ten projekt jest prostym w obsłudze, ale zaawansowanym silnikiem do gry
w szachy, który pozwoli Ci cieszyć się emocjonującymi rozgrywkami przeciwko komputerowi.

# O Autorze

Jestem Krzysiek, a Silnik Szachowy jest moim pierwszym większym
projektem napisanym w języku C++. Przez ostatnie pół roku poświęciłem mnóstwo
czasu i wysiłku na samodzielną pracę nad tym projektem.

W trakcie tworzenia Silnika Szachowego postanowiłem postawić przed sobą wyzwanie i wymyślić
wszystkie niezbędne funkcje i mechaniki samodzielnie, zamiast szukać gotowych
rozwiązań w internecie. Zależało mi na tym, aby każdy fragment kodu był moim
własnym dziełem i odzwierciedlał moje indywidualne podejście do rozwiązywania problemów.

Projekt ten jest dla mnie nie tylko okazją do praktycznego stosowania wiedzy
programistycznej, ale także dowodem na to, że jestem zdeterminowany, aby osiągnąć
swoje cele i rozwijać się jako programista.

# Co to jest za projekt?

Jest to program, który umożliwia grę w szachy z komputerem. Dzięki intuicyjnemu
interfejsowi użytkownika i prostemu menu, możesz łatwo rozpocząć grę i przetestować
swoje umiejętności szachowe.

# Oprawa graficzna

Jednym z unikalnych elementów tego projektu jest jego oryginalna oprawa graficzna,
która została stworzona bez użycia zewnętrznych bibliotek graficznych czy narzędzi.
Zamiast tego postanowiłem wykorzystać podstawowe funkcje języka C++, aby stworzyć
proste, ale efektowne prezentacje gry.

Po uruchomieniu programu użytkownik zostanie poproszony o dostosowanie konsoli do
pełnego ekranu i oddalenie, co pozwoli na wygodne wyświetlenie obrazu silnika szachowego.
Po rozpoczęciu gry, użytkownik zobaczy piksel-artową szachownicę o wymiarach 416 na 176 znaków,
gdzie każdy znak reprezentuje pojedynczy piksel.

Decyzja o tworzeniu oprawy graficznej w ten sposób wynikała z chęci uniknięcia
gotowych rozwiązań i stworzenia czegoś oryginalnego i wyjątkowego. Ten proces
pozwolił mi na rozwijanie moich umiejętności programistycznych i eksperymentowanie
z możliwościami języka C++, co w rezultacie zaowocowało wyjątkowym wizualnym
doświadczeniem dla użytkownika mojego silnika szachowego.

# Jak zacząć?

1. Pobierz pliki projektu z repozytorium.
2. Rozpakuj pliki w wybranym folderze.
3. Skompiluj projekt zgodnie z instrukcjami zawartymi w poniższej sekcji.
4. Postępuj zgodnie z instrukcjami wyświetlanymi na ekranie, aby rozpocząć grę.

# Instrukcje kompilacji

1. Przejdź do katalogu projektu: Otwórz terminal lub wiersz poleceń i przejdź do
   katalogu, w którym znajduje się pobrany projekt.

2. Skompiluj projekt: Wykonaj polecenie kompilacji odpowiednie dla Twojego systemu
   operacyjnego i używanego kompilatora (np. g++, clang++, itp.). Na przykład:

   - Dla systemu Windows:  
      g++ -o nazwa_wyjsciowa.exe main.cpp Chessboard.cpp Engine.cpp Game.cpp
     GlobalDefinitions.cpp Move.cpp Notebook.cpp Notice.cpp Play.cpp
   - Dla systemu Linux lub macOS:  
      g++ -o nazwa_wyjsciowa main.cpp Chessboard.cpp Engine.cpp Game.cpp
     GlobalDefinitions.cpp Move.cpp Notebook.cpp Notice.cpp Play.cpp

   Możesz użyć innego wybranego kompilatora C++ zależnie od preferencji.

3. Uruchomienie programu: Po pomyślnym zakończeniu kompilacji możesz uruchomić program,
   korzystając z polecenia systemowego odpowiedniego dla Twojego systemu operacyjnego
   lub klikając w powstały plik wykonywalny .exe.

# Jak grać?

- Aby zagrać, wybierz w menu głównym opcję "Zagraj", a następnie wybierz kolor,
  którym chcesz zagrać. Wykonaj swój pierwszy ruch, wpisując współrzędne bierki,
  którą chcesz poruszyć, a następnie pole, na które chcesz ją przesunąć (np. d2 d4).
- Program automatycznie zareaguje na Twój ruch i wykona go, jeśli jest poprawny,
  lub wyświetli stosowny komunikat w przypadku nieprawidłowego ruchu.
- Korzystaj z opcji menu w trakcie gry, aby dostosować ustawienia kolorów lub
  wyjść z gry, a następnie zapisać notację partii w celu późniejszego zobaczenia.
  Aby otworzyć menu, wciśnij w trakcie gry klawisz "Esc".

# Dlaczego warto spróbować?

- Prosty interfejs użytkownika zapewnia łatwą nawigację i szybki dostęp do gry.
- Personalizacja kolorów i języków pozwala dostosować grę do swoich preferencji.
- Doskonała zabawa i możliwość sprawdzenia swoich umiejętności szachowych w wygodnym środowisku.

Ciesz się grą w szachy i sprawdź, czy jesteś lepszy od mojego programu!

██████████████████████████████████████████████  
██████████████████████████████████████████████  
██████████████████████████████████████████████  
██████████████████████████████████████████████  
███████████████████████▀──████████████████████  
████████████████████▀──────▀▀█████████████████  
██████████████████▀────────────▀▀█████████████  
█████████████████▀────────────────████████████  
█████████████████──────█▄──▄▄██▄▄█████████████  
████████████████────────██████████████████████  
███████████████──────────█████████████████████  
███████████████───────────▀███████████████████  
███████████████─────────────▀█████████████████  
███████████████▄─────────────█████████████████  
████████████████▄───────────██████████████████  
█████████████████──────────███████████████████  
████████████████▀▀────────▀▀██████████████████  
██████████████▀──────────────▀████████████████  
██████████████▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄████████████████

# Chess Engine

Welcome! This project is a user-friendly yet advanced chess engine that
allows you to enjoy exciting games against the computer.

# About the Author

I'm Krzysztof Kazanecki, and the Chess Engine is my first major project written
in C++. Over the past six months, I've dedicated a lot of time and effort to working
independently on this project.

During the development of the Chess Engine, I challenged myself to come up with all
the necessary functions and mechanics independently, instead of seeking ready-made
solutions online. It was important to me that every piece of code was my own work
and reflected my individual approach to problem-solving.

This project is not only an opportunity for me to put my programming knowledge into
practice but also proof of my determination to achieve my goals and develop as a programmer.

# What is this project?

It's a program that enables you to play chess against the computer. With an intuitive
user interface and simple menu, you can easily start a game and test your chess skills.

# Graphic design

One of the unique elements of this project is its original graphic design, created
without the use of external graphic libraries or tools. Instead, I decided to utilize
basic C++ functions to create a simple yet visually appealing presentation of the game.

Upon launching the program, the user will be prompted to adjust the console to full
screen and zoom in, allowing for comfortable display of the chess engine's image.
Once the game starts, the user will see a pixel art chessboard with dimensions
of 416 by 176 characters, where each character represents a single pixel.

The decision to create the graphic design in this way stemmed from a desire to avoid
ready-made solutions and create something original and unique. This process allowed me
to develop my programming skills and experiment with the possibilities of C++, resulting
in a unique visual experience for the user of my chess engine.

# Getting started

1. Download project files from the repository.
2. Unzip the files in your chosen folder.
3. Compile the project according to the instructions provided in the section below.
4. Follow the on-screen instructions to start the game.

# Compilation Instructions

1. Navigate to the project directory: Open a terminal or command prompt and navigate
   to the directory where you downloaded the project.

2. Compile the project: Execute the compilation command appropriate for your operating
   system and compiler (e.g., g++, clang++, etc.). For example:

   - For Windows:  
      g++ -o output_name.exe main.cpp Chessboard.cpp Engine.cpp Game.cpp
     GlobalDefinitions.cpp Move.cpp Notebook.cpp Notice.cpp Play.cpp

   - For Linux or macOS:  
      g++ -o output_name main.cpp Chessboard.cpp Engine.cpp Game.cpp
     GlobalDefinitions.cpp Move.cpp Notebook.cpp Notice.cpp Play.cpp

   Or use any other C++ compiler of your choice depending on your preferences.

3. Run the program: After successful compilation, you can run the program using
   the system command appropriate for your operating system or by clicking on the
   generated executable file .exe.

# How to Play?

- To play, choose the "Play" option from the main menu and then select the color
  you want to play with. Make your first move by entering the coordinates of the piece you
  want to move and then the square you want to move it to (e.g., d2 d4).
- The program will automatically respond to your move and execute it if it's valid,
  or display a relevant message in case of an invalid move.
- Use the menu options during the game to adjust color settings or exit the game
  and save the game notation to a file. To open the menu during the game, press Esc.

# Why Give It a Try?

- The simple user interface provides easy navigation and quick access to the game.
- Customization of colors and languages allows you to tailor the game to your preferences.
- Enjoyable gameplay and the opportunity to test your chess skills in a convenient environment.

Enjoy playing chess and see if you can outsmart my program!
