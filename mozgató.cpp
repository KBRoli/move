/*
 * ============================================================================
 *
 *       Filename:  binary_tree.cpp
 *
 *    Description:  Build a binary tree from the human genom
 *
 *        Version:  0.4
 *        Created:  2014-03-23
 *   Last Updated:  2014-05-04
 *
 *         Author:  Zoltan Dalmadi
 *         E-mail:  dmz985@gmail.com
 *
 * ============================================================================
 *
 *      Changelog:
 *
 *            0.1: initial release
 *            0.2: added C++11 features
 *            0.3: added deep copy "test"
 *            0.4: added move semantics "test"
 *
 * ============================================================================
 */

#include <iostream>
#include <fstream>
#include "BT.h"

int main(int argc, const char *argv[])
{
  // ==========================================================================

  // Leellen�rizz�k a megadott param�tereket
  // Ha a param�terek sz�ma nem 3 (programn�v + bemeneti file + kimeneti file)
  if (argc != 3)
  {
    // Ha van bemeneti file, de nincs kimeneti
    if (argv[1] && !argv[2])
    {
      // Ki�rjuk a helyes haszn�latot a standard error-ra
      std::cerr << "Nincs kimeneti file!" << std::endl \
        << "Hasznalat: " << argv[0] << " bemeneti_file kimeneti_file" \
        << std::endl;

      // �s megszak�tjuk programunk m�k�d�s�t
      return -1;
    }

    // Ha nincs se bemeneti, se kimeneti file
    std::cerr << "Hasznalat: " << argv[0] << " bemeneti_file kimeneti_file" \
      << std::endl;

    // �s megszak�tjuk programunk m�k�d�s�t
    // Az�rt k�l�nb�zik a 2 return sz�m, mert k�l�nb�z� esetekre tudn�nk �gy
    // hivatkozni, ha implement�ln�nk komolyabb hibekezel� elj�r�sokat. Ebben
    // a programban nincs r� sz�ks�g, de j� szok�s k�l�n hibak�ddal kezelni
    // k�l�nb�z� hibaeseteket. A hib�s eseteket gyakran jel�lj�k negat�v
    // sz�mokkal, mert k�nnyen felismerhet�ek pozit�v t�rsaik mellett
    return -2;
  }

  // ==========================================================================

  // L�trehozzuk a bemeneti file-t kezel� objektumunkat
  std::ifstream input(argv[1]);

  // Megvizsg�ljuk, hogy a bemeneti file olvashat�-e (l�tezik-e)
  if (!input)
  {
    std::cerr << "Bemeneti file nem olvashato!" << std::endl;
    return -3;
  }

  // Megvizsg�ljuk, hogy a bemeneti file �res-e. Erre a be�p�tett ifstream
  // oszt�ly peek() f�ggv�ny�t haszn�ljuk, ami annyit csin�l, hogy beolvassa
  // a file k�vetkez� karakter�t an�lk�l, hogy elt�roln�. Ha k�vetkez�
  // (ebben az esetben az els�) karakter EOF (End of File, azaz file a v�g�t
  // jelz� karakter), akkor a file �res
  if (input.peek() == EOF)
  {
    std::cerr << "Bemeneti file ures!" << std::endl;
    return -4;
  }

  // L�trehozzuk a kimeneti file-t kezel� objektumot is
  std::ofstream output(argv[2]);

  // ==========================================================================

  // Dinamikusan foglalunk mem�ri�t az aktu�lisan beolvasott karakternek
  char* currentChar = new char;

  // L�trehozzuk a bin�ris fa objektumunkat
  BT BinTree;

  // Komment-e az aktu�lis karakter�nk?
  bool isComment = false;

  // Elkezdj�k a beolvas�si proced�r�t
  while(input.read(currentChar, sizeof(char)))
  {
    // Megvizsg�ljuk az aktu�lis karaktert
    switch (*currentChar)
    {
      // Ha >, akkor az egy komment elej�t jelenti a file-ban
      case '>':

        // teh�t a komment v�ltoz�nkat igazra �ll�tjuk
        isComment = true;

        // �s folytatjuk a ciklus k�vetkez� l�p�s�vel,
        // vagyis l�p�nk a k�vetkez� karakterre
        continue;

      // Ha sor v�gekarakterhez �rt�nk, ott v�ge a kommentnek
      case '\n':

        // teh�t a komment v�ltoz�nkat hamisra �ll�tjuk
        isComment = false;

        // �s ugyan�gy l�p�nk a k�vetkez� karakterre
        continue;

      // Az N bet�k nem hordoznak inform�ci�t, �gy azokat is �tl�pj�k
      case 'N':
        continue;
    }

    // Ha a komment v�ltoz�nk igaz, az azt jelenti, hogy az aktu�lis karakter
    // �ppen egy komment. Ugyan�gy l�p�nk a k�vetkez� karakterre
    // (sorv�ge karakterig, l�sd fentebb)
    if(isComment)
    {
      continue;
    }

    // Az aktu�lis karakter bitjeit behelyezz�k a f�ba, a t�lterhelt <<
    // oper�torunk seg�ts�g�vel. Haszn�lhatn�nk az addNode() f�ggv�nyt is.
    // Az�rt 8-ig iter�lunk, mert a char adatt�pus 8 bites
    for (int i = 0; i < 8; i++)
    {
      // Bitmaszkol�ssal a legfels� bitet megvizsg�ljuk. A maszk 0x80, azaz
      // 1000_0000. Ha egy bitet 1-el �S-el�nk, akkor mindig megkapjuk az
      // eredeti bit�nket. �gy ezzel a m�dszerrel meg tudjuk �llap�tani a
      // maszkban az 1-es hely�n �ll� bit poz�ci�j�n�l (ami ebben az esetben
      // a legfels� bit) a karakter�nk bitj�nek �rt�k�t
      if(*currentChar & 0x80)
      {
        // Ha a logikai �S m�velet igaz igazs�g�rt�kkel b�rt, az azt jelenti,
        // hogy az aktu�lis karakter legfels� bitj�n 1-es van, teh�t a bin�ris
        // f�nkba 1-est kell helyezni
        BinTree << '1';
      }
      else
      {
        // Ellenkez� esetben 0 volt a legfels� bit, �gy 0-t helyez�nk el
        BinTree << '0';
      }
      // �s az aktu�lis karakteren biteltol�st hajtunk v�gre balra, hogy a
      // k�vetkez� bitje ker�lj�n a legfels� poz�ci�ba
      *currentChar <<= 1;
    }
  }

  // ==========================================================================

  // Mivel a bemeneti file-ra m�r nincs sz�ks�g�nk, �gy lez�rjuk azt
  input.close();

  // Megh�vjuk a f�nk ki�r� f�ggv�ny�t
  BinTree.printAllNodes(BinTree.getRoot(), output);

  // Megh�vjuk a m�lys�get ki�r� f�ggv�nyt
  BinTree.printHeight(BinTree.getRoot(), output);

  // Megh�vjuk az �tlagot ki�r� f�ggv�nyt
  BinTree.printAverage(BinTree.getRoot(), output);

  // Megh�vjuk a sz�r�st ki�r� f�ggv�nyt
  BinTree.printVariance(BinTree.getRoot(), output);

  // ========================== M�SOL�S "TESZT" ===============================

  // P�ld�nyos�tunk egy �j binfa objektumot
  BT BinTree2;

  // Az egyenl�s�gjel oper�torral megh�v�dik a m�sol� �rt�kad�s.
  // Mivel el�tte p�ld�nyos�tottuk az �j objektumot, �gy az m�r l�tezik, �s
  // nem a m�sol� konstruktor h�v�dik meg, hanem a t�lterhelt egyenl�s�gjel
  // oper�torunk.
  // Ha p�ld�nyos�t�s k�zben m�soln�nk (BT BinTree2 = BinTree), akkor a norm�l
  // m�sol� konstruktor h�v�dna meg, mivel a m�sodik objektumot el�tte
  // l�tre kell hozni

  BinTree2 = BinTree;

  // Megh�vjuk a m�solt objektumra is a ki�r� f�ggv�nyeket.
  // J�l l�that� a kimeneti file-ban, hogy a f�k tartalma megegyezik, azonban
  // minden egyes csom�pont c�me k�l�nb�z�.
  output << std::endl << "******* M�SOLAT *******" << std::endl << std::endl;
  BinTree2.printAllNodes(BinTree2.getRoot(), output);
  BinTree2.printHeight(BinTree2.getRoot(), output);
  BinTree2.printAverage(BinTree2.getRoot(), output);
  BinTree2.printVariance(BinTree2.getRoot(), output);

  // ========================= MOZGAT�S "TESZT" ===============================

  // P�ld�nyos�tunk egy �j binfa objektumot
  BT BinTree3;

  // Hasonl�an a m�sol� konstruktorhoz, ha az objektumunk a mozgat�s el�tt m�r
  // l�tezett, akkor a mozgat� �rt�kad�s h�v�dik meg (ami nem m�s, mint az
  // egyenl�s�gjel oper�torunk egy �jabb t�lterhel�se)
  // Ebben az esetben is, ha p�ld�nyos�t�s k�zben mozgatn�nk, akkor a mozgat�
  // konstruktort h�vn� meg.
  // std::move f�ggv�ny: technikailag a param�ter�ben megadott bal�rt�kb�l
  // jobb�rt�ket csin�l, aminek eredm�nye a mozgat� konstruktor/�rt�kad�s
  // megh�v�d�sa
  BinTree3 = std::move(BinTree);

  // Megh�vjuk a mozgatott objektumra is a ki�r� f�ggv�nyeket.
  // A kimeneten l�that�, hogy a mozgatott fa csom�pontjainak c�mei megegyeznek
  // az els� f�nk csom�pontjainak c�meivel, teh�t sikeres volt a mozgat�s
  output << std::endl << "******* MOZGATOTT *******" << std::endl << std::endl;
  BinTree3.printAllNodes(BinTree3.getRoot(), output);
  BinTree3.printHeight(BinTree3.getRoot(), output);
  BinTree3.printAverage(BinTree3.getRoot(), output);
  BinTree3.printVariance(BinTree3.getRoot(), output);

  // ==========================================================================

  // Nem haszn�ljuk tov�bb a kimeneti filet, �gy lez�rjuk
  output.close();

  // t�r�lj�k az aktu�lis karaktert t�rol� v�ltoz�t, illetve az �ltala
  // haszn�lt dinamikusan lefoglalt mem�ri�t felszabad�tjuk
  delete currentChar;
  currentChar = nullptr;

  return 0;
}