#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;
int MainHP, MainHPMax, MaxStage, CurrStage, Mana, Element, MaxEnemies = 2;
float Dmg, Status, Defence, DamageMult, StatusMult;

void SetColor(int color) {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, color);
}

float Clamp(float num, float min, float max) {
  if (num > max) {
    return max;
  }
  if (num < min) {
    return min;
  }
  return num;
}

class Enemy {
 public:
  Enemy(int ID);
  bool Alive = true;
  int Type, IsBoss = 0, DamageRand = 0;
  void Attack();
  void Stats();
  void RecieveDmg();
  void virtual CheckHP();
  void virtual Act();
  float HP = 0, HPMax = 0, Damage = 0, Resist = 1, FlameResist = 1,
        FrostResist = 1, DarkResist = 1, PsychoResist = 1, Flame = 0, Frost = 0,
        Dark = 0, Psycho = 0;
  ;
  string Name, AtkText1, AtkText2, AtkText3, AtkText4, CalmText, HurtText,
      DamagedText, DeathText;
};

Enemy::Enemy(int ID = 0) { 
  Type = ID;
  switch (Type) { 
    case 0:
      Name = "Arms cluster";
      HP = HPMax = 20;
      Damage = 5;
      DamageRand = 10;
      PsychoResist = 0.5;
      FlameResist = 0.5;
      FrostResist = 0.5;
      DarkResist = 0.5;
      AtkText1 = "Arms cluster strike";
      AtkText2 = "Arms cluster roll towards Mage";
      AtkText3 = AtkText1;
      AtkText4 = AtkText2;
      CalmText = "Arms cluster move slowly";
      HurtText = "Arms cluster shake";
      DamagedText = "Arms cluster barely stand";
      DeathText = "Arms cluster fell to dust";
  }
}

void Enemy::Act() { 
  HP -= Flame;
  CheckHP();
  Attack();
}

void Enemy::Attack() {
  int AtkDamage = (Damage + rand() % (DamageRand + 1)) * (1 - Frost / 100);
  if (rand() % 100 >= Psycho) {
    if (rand() % 2 == 0) {
      if (rand() % 2 == 0) {
        cout << AtkText1;
      } else {
        cout << AtkText2;
      }
    } else {
      if (rand() % 2 == 0) {
        cout << AtkText3;
      } else {
        cout << AtkText4;
      }
    }
    if (rand() % 100 >= Dark) {
      cout << " which inflict " << AtkDamage * Defence
           << " damage to "
              "Last Mage\n";
      MainHP -= AtkDamage * Defence;
    } else {
      cout << ". Miss\n";
    }
  } else {
      cout << Name << " in psychotic assault hurt self with " << AtkDamage * (Resist)
           << " damage\n";
      HP -= AtkDamage;
    }
}

void Enemy::Stats() {
  CheckHP();
  if (HP >= HPMax / 2) {
    cout << CalmText + "\n";
  } else {
    if (HP >= HPMax / 4) {
      cout << HurtText + "\n";
    } else {
      cout << DamagedText + "\n";
    }
  }
}

void Enemy::RecieveDmg() {
  HP -= Dmg * DamageMult * Resist;
  cout << Name + " got hit - " << (Dmg * DamageMult * Resist) << " damage\n";
  switch (Element) {
    case 1:
      Flame += Status * StatusMult * FlameResist;
      if (Flame >= 50) {
        Flame = 0;
        HP -= 100;
        cout << "Flame overload! " << Name << " exploded";
      }
    case 2:
      Frost += Status * StatusMult * FrostResist;
      if (Frost >= 50) {
        Frost = 0;
        HP -= 100;
        cout << "Frost overload! " << Name << " were frozen";
      }
    case 3:
      Dark += Status * StatusMult * DarkResist;
      if (Frost >= 50) {
        Frost = 0;
        HP -= 100;
        cout << "Dark overload! " << Name << " got corrupted";
      }
    case 4:
      Psycho += Status * StatusMult * PsychoResist;
      if (Frost >= 50) {
        Frost = 0;
        HP -= 100;
        cout << "Psycho overload! " << Name << " attacked self in hysteria";
      }
  }
  CheckHP();
}

class Boss : public Enemy {
 public:
  Boss(int ID);
  int SpecAttackChance = 30;
  string Hint = "";
  bool SpecFlag = false;
  void SpecialAttack();
  void virtual Act();
  void virtual CheckHP();
};

Boss::Boss(int ID = 0) {
  Type = ID;
  IsBoss = 1;
  switch (Type) {
    case 0:
      SetColor(8);
      cout << "Two guardians of castle stand before Last Mage. One raise his "
              "heavy shield, another prepared "
              "battle axe. Fight is only "
              "option\n";
      SetColor(7);
      HP = HPMax = 100;
      Damage = 5;
      Resist = 0.6;
      DamageRand = 10;
      Name = "Shield guardian";
      AtkText1 = "Shield guardian attack with shield";
      AtkText2 = "Shield guardian do a shield bash";
      AtkText3 = "Shield guardian slash with knife";
      AtkText4 = "Shield guardian throw small knife";
      CalmText = "Shield guardian stand still behind his shield";
      HurtText = "Shield guardian shake behind his shield";
      DamagedText =
          "Shield guardian barely stand, only shield don't let him fall";
      DeathText = "Shield guardian fell on knees breathless";
      Hint = "Shield guardian have resist to direct damage, but no more";
      break;
    case 1:
      HP = HPMax = 100;
      Damage = 10;
      DamageRand = 5;
      SpecAttackChance = 20;
      PsychoResist = 0.9;
      FlameResist = 0.9;
      FrostResist = 0.9;
      DarkResist = 0.9;
      Name = "Axe guardian";
      AtkText1 = "Axe guardian do a heavy slash";
      AtkText2 = "Axe guardian do two fast slashes";
      AtkText3 = "Axe guardian do a spin slash";
      AtkText4 = "Axe guardian attack with axe";
      CalmText = "Axe guardian look in Last Mage eyes";
      HurtText = "Axe guardian bleeds";
      DamagedText = "Axe guardian tries to stay focused";
      DeathText = "Axe guardian tried to attack. He fell before Last Mage";
      Hint = "Axe guardian have small resists to all elements. Looks like this is effect of his ring";
      break;
    case 2:
      SetColor(8);
      cout << "The palace smell dry. In center of it stands mummified witch "
              "corpse with eight arms. As Mage get closer she rise her gaze to "
              "Last Mage, her dead eyes glow red. This is mad witch who "
              "cheated death now known as ";
      SetColor(4);
      cout << "InfArmY";
      SetColor(8);
      cout << " - supreme ripper\n";
      SetColor(7);
      HP = HPMax = 300;
      Damage = 1;
      DamageRand = 39;
      PsychoResist = 1.4;
      FlameResist = 1.2;
      FrostResist = 0.8;
      DarkResist = 0.8;
      Name = "InfArmY";
      AtkText1 = "InfArmY attack with all her arms";
      AtkText2 = "Dead Witch deal multiple blows with her arms";
      AtkText3 = "Living corpse stand on arms and do fast kicks";
      AtkText4 = "Mummy create and throw auxiliary arm";
      CalmText = "InfArmY's arms sway in chaotic rhytm";
      HurtText = "Living Corpse bursts into laughter";
      DamagedText = "Witch shakes with malicious smile on her face";
      DeathText = "Mummy grab and rend herself apart spreading dust around, all her body and auxilary arms turned to dust";
      Hint = "Mad Witch have frost and dark resists, but flame works good against her mummyfied body. Madness make psycho effective against her";
      break;
    case 3:
      SetColor(8);
      cout << "As Last Mage enter the palace, smoke fill it. As steam cleared, Gorgeous machine stood before Last Mage. ";
      SetColor(14);
      cout << "GO-13M. Greatest clockwork mechanism.";
      SetColor(8);
      cout << " It's a pity that such perfection is enemy of the Mage\n";
      SetColor(12);
      cout << "- Powerful treat detected. Starting battle\n ";
      SetColor(7);
      HP = HPMax = 400;
      Damage = 20;
      DamageRand = 20;
      SpecAttackChance = 40;
      Resist = 0.5;
      PsychoResist = 0.4;
      FlameResist = 0.8;
      FrostResist = 1.2;
      DarkResist = 1.2;
      Name = "GO-13M";
      AtkText1 = "GO-13M do a powerful kick";
      AtkText2 = "Machine rush towards Mage";
      AtkText3 = "Clockwork mechanism strike with high pressure steam";
      AtkText4 = "Mechanism strike with his arms";
      CalmText = "Machine click in perfect rhytm";
      HurtText = "GO-13M stand still";
      DamagedText = "Mechanism rhytm became unstable";
      DeathText = "GO-13M released lots of steam from spine";
      Hint = "Mechanical mind have strong psycho resist, also strong resist to direct attack damage. Good flame resist. Frost and dark are effective";
      break;
    case 4:
      HP = HPMax = 150;
      Damage = 5;
      DamageRand = 10;
      SpecAttackChance = 60;
      Resist = 0.9;
      PsychoResist = 1.2;
      FlameResist = 1.2;
      FrostResist = 1.2;
      DarkResist = 1.2;
      Name = "Wings";
      AtkText1 = "Wings strike with sharp feather";
      AtkText2 = "Wings do a spinnig attack";
      AtkText3 = "Wings strike with sharp feather";
      AtkText4 = "Wings do a spinnig attack";
      CalmText = "Wings fly near the Last Mage";
      HurtText = "Wings wave became unstable";
      DamagedText = "Wings shake in the air";
      DeathText = "Wings fell on ground";
      Hint = "Wings have small resist to direct damage. Elements are effective against it";
      break;
    case 5:
      SetColor(8);
      cout
          << "Gorgeous creature were watching in mosaic window as Last Mage "
             "appeared. It slowly turns to manifest itself. Thin legs levitate "
             "above ground, Halo shines with purple, Wings spread wide and "
             "separate from body, long "
             "tail curl around thin arm and purple core shine inside its "
             "chest. Round dark helmet hide its head. Creature look too weird "
             "to be a human. This is the final battle. Battle with\n";
      SetColor(13);
      cout << "TYRANT";
      SetColor(8);
      cout << " - the order creator and the first slayer\n";
      SetColor(7);
      HP = HPMax = 300;
      Damage = 10;
      DamageRand = 20;
      SpecAttackChance = 30;
      Resist = 0.8;
      PsychoResist = 0.8;
      FlameResist = 0.8;
      FrostResist = 0.8;
      DarkResist = 0.8;
      Name = "Tyrant";
      AtkText1 = "Tyrant tail do a powerful stab";
      AtkText2 = "Creature fly towards mage and kick on full speeed";
      AtkText3 = "Thin arms strike with surprising power";
      AtkText4 = "The first slayer do multiples slashes with its tail";
      CalmText = "Creature slowly move up and down in air";
      HurtText = "Tyrant growl";
      DamagedText = "Helmet crack, it's growl rise";
      DeathText = "Tyrant helmet explode opening up a view of dark emptiness inside. Body fall on ground";
      Hint = "Tyrant have low resist to all elements and direct damage";
      break;
    case 6:
      HP = HPMax = 100;
      Damage = 2;
      DamageRand = 2;
      SpecAttackChance = 80;
      Resist = 1.2;
      PsychoResist = 0;
      FlameResist = 1.2;
      FrostResist = 1.2;
      DarkResist = 0;
      Name = "Halo";
      AtkText1 = "Halo strike with light";
      AtkText2 = "Halo attack with lightning";
      AtkText3 = "Halo strike with light";
      AtkText4 = "Halo attack with lightning";
      CalmText = "Halo shines with purple";
      HurtText = "Halo cracks";
      DamagedText = "Halo cracks shine white";
      DeathText = "Halo shine bright and slowly dissapear";
      Hint = "Halo have overresist to dark and psycho - they are useless. Anything else will be effective";
      break;
  }
}

vector<Enemy> Enemies;
vector<Boss> StageBosses;
Enemy* Target = 0;

void Boss::SpecialAttack() {
  switch (Type) {
    case 0:
      cout << "Shield guardian used heal potion - restored 20 health\n";
      HP = Clamp(HP + 20, 0, HPMax);
      break;
    case 1:
      cout << "Axe guardian looked at ring on his arm. His elemental statuses "
              "decreased slightly\n";
      Flame = Clamp(Flame - 3, 0, 100);
      Frost = Clamp(Frost - 3, 0, 100);
      Dark = Clamp(Dark - 3, 0, 100);
      Psycho = Clamp(Psycho - 3, 0, 100);
      break;
    case 2:
      if (Enemies.size() < MaxEnemies) {
        cout << "Witch created cluster of arms\n";
        Enemies.push_back(Enemy(0));
      } else {
        cout << "Witch tried to create more arms but overloaded herself - 20 damage\n";
        HP -= 20;
        CheckHP();
      }
      break;
    case 3:
      cout << "Golem releases steam\n";
      SetColor(12);
      cout << "- Overheat. Processing...\n";
      SetColor(7);
      break;
    case 4:
      cout << "Wings throw feathers around. Feathers decrease enemy statuses\n";
      for (Boss& boss : StageBosses) {
        boss.Flame = Clamp(boss.Flame - 3, 0, 100);
        boss.Frost = Clamp(boss.Frost - 3, 0, 100);
        boss.Dark = Clamp(boss.Dark - 3, 0, 100);
        boss.Psycho = Clamp(boss.Psycho - 3, 0, 100);
      }
      break;
    case 5:
      if (SpecFlag){
        cout << "Tyrant emits a mighty roar and strike with all limbs. ";
        float AtkDamage = (40 + rand() % (11)) * (1 - Frost / 100) * Defence;
        cout << "That inflict " << AtkDamage << " damage to Last Mage\n";
        MainHP -= AtkDamage;
        SpecFlag = false;  
      }else {
        cout << "Tyrant prepares something, her core glow bright\n";
        SpecFlag = true;
      }
      break;
    case 6:
      cout << "Halo shine with purple light. Light heal all enemies by 20 health\n";
      for (Boss& boss : StageBosses) {
        boss.HP = Clamp(boss.HP + 20, 0, boss.HPMax);
      }
      break;
  }
}

void Boss::Act() {
  HP -= Flame;
  CheckHP();
  if (rand() % 100 >= SpecAttackChance) {
    Attack();
  } else {
    SpecialAttack();
  }
}

void Enemy::CheckHP() {
  if (HP <= 0) {
    cout << DeathText << "\n";
    Alive = false;
    erase_if(Enemies, [](const Enemy& b) { return !b.Alive; });
  }
}

void Boss::CheckHP() {
  if (HP <= 0) {
    cout << DeathText << "\n\n";
    Alive = false;
    erase_if(StageBosses, [](const Boss& b) { return !b.Alive; });
    if (StageBosses.empty()) {
      if (!Enemies.empty()) {
        Enemies.clear();
      }
      switch (CurrStage) {
        case 0:
          cout << "Axe guardian ring shines bright\n";
          SetColor(2);
          cout << "Ring of memories obtained\n\n";
          SetColor(7);
          break;
        case 1:
          cout << "Ring shines in the dust\n";
          SetColor(2);
          cout << "Ring of arms obtained\n\n";
          SetColor(7);
          break;
        case 2:
          SetColor(12);
          cout << "- Overheat! Overheat! Overheat...\n";
          SetColor(7);
          cout << "As machine turned off, its chest opened. Inside was a ring\n";
          SetColor(2);
          cout << "Clockwork ring obtained\n\n";
          SetColor(7);
          break;
        case 3:
          SetColor(10);
          cout << "\nYou finished the mission - you destroyed the order's core\n"
                  "Congratulations, Last Mage\n\n\n";
          SetColor(7);
          exit(0);
      }
      MaxStage++;
      CurrStage++;
    }
  }
}

void Create_enemies() {
  switch (CurrStage) {
    case 0:
      StageBosses.push_back(Boss(0));
      StageBosses.push_back(Boss(1));
      break;
    case 1:
      StageBosses.push_back(Boss(2));
      break;
    case 2:
      StageBosses.push_back(Boss(3));
      break;
    case 3:
      StageBosses.push_back(Boss(4));
      StageBosses.push_back(Boss(5));
      StageBosses.push_back(Boss(6));
      break;
  }
}

void Magic();

void Offence();

void EnemyChooser() {
  cout << "Choose enemy\n";
  int Chosen;
  bool flag = true;
  do {
    while (!(cin >> Chosen)) {
      cout << "Invalid target\n";
    }
    if ((Chosen < 1) or Chosen > (Enemies.size() + StageBosses.size())){
      cout << "Invalid target!\n";
    } else{
      if (Chosen <= StageBosses.size()) {
        Target = &StageBosses[Chosen - 1];
      } else {
        Target = &Enemies[Chosen - StageBosses.size() - 1];
      }
      flag = false;
    }
  } while (flag);
}

int main() {
  srand(time(NULL));
  SetColor(8);
  cout << "You are the Last Mage. \nYour goal is simple - revenge."
          "To revenge for all the order slayed - to destroy their mage "
          "slayers.\n";
  SetColor(4);
  cout << "Once and for all.\n\n";
  SetColor(7);
  CurrStage = MaxStage = 0;
  MainHP = MainHPMax = 100;
  Mana = 100;
  Defence = 1;
  int State = 2, Choice;
  float ManaMax = 100;
  while (MainHP > 0) {
    switch (State) {
      case 1:
        Mana = ManaMax;
        MainHP = MainHPMax;
        switch (CurrStage) { 
          case 1:
            cout << "Spacious outer palaces look regular\n";
            break;
          case 2:
            cout << "Spacious inner palaces shine before Last Mage\n";
            break;
          case 3:
            cout << "Slayers section feels majestically. Soon it will burn\n";
            break;
        }
        SetColor(11);
        cout << "\nLast Mage\n";
        SetColor(15);
        cout << "1. Inventory\n2. Go to the next palace\n3. Right artifact "
                "room\n4. Left Artifact room\n";
        if (CurrStage != 1) {
          cout << "5. Return to previous palace\n";
        }
        cin >> Choice;
        switch (Choice) {
          case 1:
            system("cls");
            break;
          case 2:
            if (CurrStage >= MaxStage) {
              State = 2;
            } else {
              CurrStage++;
            }
            system("cls");
            break;
          case 3:
            system("cls");
            break;
          case 4:
            system("cls");
            break;
          case 5:
            if (CurrStage != 1) {
              CurrStage -= 1;
              system("cls");
              break;
            }
          default:
            system("cls");
            cout << "That didn't work\n";
            break;
        }
        break;
      case 2:
        Create_enemies();
        do {
          int counter = 1;
          Defence = 1;
          DamageMult = StatusMult = 1;
          SetColor(4);
          cout << "\nEnemies\n";
          SetColor(7);
          for (Boss& boss : StageBosses) {
            cout << counter << ". ";
            counter++;
            boss.Stats();
          }
          for (Enemy& enemy : Enemies) {
            cout << counter << ". ";
            counter++;
            enemy.Stats();
          }
          SetColor(11);
          cout << "\nLast Mage\n";
          SetColor(15);
          cout << "Health: ";
          SetColor(4);
          cout << MainHP;
          SetColor(15);
          cout << " Mana: ";
          SetColor(3);
          cout << Mana << "\n";
          SetColor(15);
          cout << "1. Regenerate. Gain ";
          SetColor(3);
          cout << "15\n";
          SetColor(15);
          if (Mana >= 20) {
            cout
                << "2. Support with spell. ";
            SetColor(3);
            cout << "20\n";
            SetColor(15);
            cout << "3. Attack with one element. ";
            SetColor(3);
            cout << "20\n";
            SetColor(15);
            if (Mana >= 40) {
              cout << "4. Attack with two elements. ";
              SetColor(3);
              cout << "40\n";
              SetColor(15);
            }
          }
          SetColor(15);
          cin >> Choice;
          switch (Choice) {
            case 1:
              Mana = Clamp(Mana + 15, 0, ManaMax);
              system("cls");
              cout << "Last Mage started regenerating\n";
              break;
            case 2:
              if (Mana >= 20) {
                Mana -= 20;
                Offence();
                break;
              }
            case 3:
              if (Mana >= 20) {
                Mana -= 20;
                EnemyChooser();
                Magic();
                break;
              }
            case 4:
              if (Mana >= 40) {
                EnemyChooser();
                Mana -= 40;
                cout << "Choose catalyst\n";
                cout
                    << "1. Flame. Ignite element. Doesn't damage but increase "
                       "status much\n";
                cout
                    << "2. Frost. Break element. Increase damage and status\n";
                cout << "3. Dark. Nullify element. Doesn't create status but "
                        "make damage bigger\n";
                cout << "4. Psycho. Overload element. Deal low heal to enemy "
                        "but add large status\n";
                cin >> Choice;
                switch (Choice) {
                  case 1:
                    DamageMult = 0;
                    StatusMult = 3;
                    break;
                  case 2:
                    DamageMult = 1.5;
                    StatusMult = 1.5;
                    break;
                  case 3:
                    DamageMult = 3;
                    StatusMult = 0;
                    break;
                  case 4:
                    DamageMult = -1;
                    StatusMult = 4.5;
                    break;
                  default:
                    system("cls");
                    cout << "That didn't work\n";
                    break;
                    break;
                }
                Magic();
                break;
              }
            default:
              system("cls");
              cout << "That didn't work\n";
              break;
          }
          for (Boss& boss : StageBosses) {
            boss.Act();
          }
          for (Enemy& enemy : Enemies) {
            enemy.Act();
          }
          if (MainHP <= 0) {
            SetColor(4);
            cout << "\nThe Last Mage fell.\n\n";
            SetColor(15);
            exit(0);
          }
          Mana = Clamp(Mana + 15, 0, ManaMax);
        } while (not StageBosses.empty());
        State = 1;
    }
  }
}

void Offence() {
    int ChoiceElem;
    cout << "Choose spell:\n";
    cout << "1. Steel blood. Decrease incoming damage by half\n";
    cout << "2. Blue heart. Heal ";
    SetColor(4);
    cout << "50\n";
    SetColor(15);
    cout << "3. Radiance. Heal ";
    SetColor(4);
    cout << "80";
    SetColor(15);
    cout << " but incoming damage rise by half\n";
    cin >> ChoiceElem;
    system("cls");
    switch (ChoiceElem) {
      case 1:
        Defence = 0.5;
        system("cls");
        cout << "Last mage blood became gray. His protction rise\n";
        break;
      case 2:
        MainHP = Clamp(MainHP + 50, 0, MainHPMax);
        system("cls");
        cout << "Last Mage heart beat strong and slow, regenerating his health\n";
        break;
      case 3:
        MainHP = Clamp(MainHP + 80, 0, MainHPMax);
        Defence = 1.5;
        system("cls");
        cout << "Last Mage heart glow inside his chest, refilling his health. He is vulnerable now\n";
        break;
      default:
        system("cls");
        cout << "That didn't work\n";
        break;
        break;
    }
}

void Magic() {
  int ChoiceElem;
  cout << "Choose element:\n";
  cout << "1. Flame. Burn enemy slowly\n";
  cout << "2. Frost. Make enemy attacks weaker\n";
  cout << "3. Dark. With darken mind enemy may miss\n";
  cout << "4. Psycho. Enemy may lose control and attack self\n";
  cin >> ChoiceElem;
  switch (ChoiceElem) {
    case 1:
      Dmg = 200;
      Status = 1;
      Element = 1;
      break;
    case 2:
      Dmg = 25;
      Status = 0.75;
      Element = 2;
      break;
    case 3:
      Dmg = 15;
      Status = 1.25;
      Element = 3;
      break;
    case 4:
      Dmg = 10;
      Status = 1.5;
      Element = 4;
      break;
    default:
      system("cls");
      cout << "That didn't work\n";
      break;
      break;
  }
  system("cls");
  Target->RecieveDmg();
}