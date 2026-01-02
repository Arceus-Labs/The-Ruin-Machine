#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define BTN_BET 13
#define BTN_MODE 12
#define BUZZER 4

const int STARTING_BANKROLL = 10000;
const int BASE_BET = 100;
const float HOUSE_EDGE = 0.495;

int bankroll = STARTING_BANKROLL;
int currentBet = BASE_BET;
int gamesPlayed = 0;
int gamesWon = 0;
int currentStreak = 0;
int longestWinStreak = 0;
int longestLossStreak = 0;
bool lastGameWon = false;

enum Strategy { MARTINGALE, FLAT_BET };
Strategy currentStrategy = MARTINGALE;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  pinMode(BTN_BET, INPUT_PULLUP);
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  
  if(display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    showStartupScreen();
    delay(2000);
    updateDisplay();
  }
}

void loop() {
  // BET button
  if (digitalRead(BTN_BET) == LOW) {
    delay(50);
    if (digitalRead(BTN_BET) == LOW) {
      if (bankroll >= currentBet) {
        playGame();
      } else {
        showInsufficientFunds();
      }
      while(digitalRead(BTN_BET) == LOW);
    }
  }
  
  // MODE button - tap to switch, hold 5s to reset
  if (digitalRead(BTN_MODE) == LOW) {
    delay(50);
    if (digitalRead(BTN_MODE) == LOW) {
      unsigned long pressStart = millis();
      bool isLongPress = false;
      int countdown = 5;
      
      while(digitalRead(BTN_MODE) == LOW) {
        unsigned long pressDuration = millis() - pressStart;
        
        if (pressDuration >= 5000 && !isLongPress) {
          isLongPress = true;
          resetGame();
          break;
        } else if (pressDuration >= 1000 && pressDuration < 5000) {
          int newCountdown = 5 - (pressDuration / 1000);
          if (newCountdown != countdown) {
            countdown = newCountdown;
            showResetCountdown(countdown);
            tone(BUZZER, 800, 50);
          }
        }
        delay(10);
      }
      
      if (!isLongPress) {
        switchStrategy();
      }
      
      while(digitalRead(BTN_MODE) == LOW);
    }
  }
  
  if (bankroll <= 0) {
    handleBankruptcy();
  }
}

void playGame() {
  float randomValue = (float)esp_random() / (float)UINT32_MAX;
  bool won = randomValue < HOUSE_EDGE;
  
  gamesPlayed++;
  
  if (won) {
    bankroll += currentBet;
    gamesWon++;
    
    tone(BUZZER, 1000, 100);
    delay(120);
    tone(BUZZER, 1500, 100);
    delay(120);
    
    if (lastGameWon) {
      currentStreak++;
    } else {
      currentStreak = 1;
    }
    lastGameWon = true;
    
    if (currentStreak > longestWinStreak) {
      longestWinStreak = currentStreak;
    }
    
    showWinScreen();
    delay(500);
    
    if (currentStrategy == MARTINGALE) {
      currentBet = BASE_BET;
    }
    
  } else {
    bankroll -= currentBet;
    
    tone(BUZZER, 400, 150);
    delay(170);
    tone(BUZZER, 200, 150);
    delay(170);
    
    if (!lastGameWon) {
      currentStreak++;
    } else {
      currentStreak = 1;
    }
    lastGameWon = false;
    
    if (currentStreak > longestLossStreak) {
      longestLossStreak = currentStreak;
    }
    
    showLossScreen(currentBet);
    delay(500);
    
    if (currentStrategy == MARTINGALE) {
      if (bankroll >= currentBet * 2) {
        currentBet *= 2;
      } else {
        if (bankroll > 0) {
          currentBet = min(bankroll, currentBet);
        }
      }
    }
  }
  
  updateDisplay();
}

void showStartupScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 5);
  display.println("GAMBLING");
  display.setCursor(25, 25);
  display.println("RUIN");
  display.setTextSize(1);
  display.setCursor(15, 50);
  display.println("Shantanu 2026");
  display.display();
}

void showWinScreen() {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 20);
  display.println("WIN!");
  display.display();
}

void showLossScreen(int amount) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 15);
  display.println("LOSS!");
  display.setTextSize(1);
  display.setCursor(25, 40);
  display.print("-Rs.");
  display.println(amount);
  display.display();
}

void showInsufficientFunds() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println("NOT ENOUGH");
  display.setCursor(25, 25);
  display.println("MONEY!");
  display.setCursor(5, 45);
  display.print("Need: Rs.");
  display.println(currentBet);
  display.setCursor(5, 55);
  display.print("Have: Rs.");
  display.println(bankroll);
  display.display();
  
  tone(BUZZER, 300, 300);
  delay(350);
  tone(BUZZER, 200, 300);
  delay(350);
  
  delay(1500);
  updateDisplay();
}

void showResetCountdown(int seconds) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println("RESET IN");
  display.setTextSize(4);
  display.setCursor(50, 30);
  display.println(seconds);
  display.display();
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("=RUIN MACHINE=");
  
  display.setTextSize(2);
  display.setCursor(0, 12);
  display.print("Rs.");
  display.println(bankroll);
  
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print("Games:");
  display.print(gamesPlayed);
  
  if (gamesPlayed > 0) {
    display.print(" ");
    display.print((float)gamesWon / gamesPlayed * 100, 1);
    display.println("%");
  } else {
    display.println("");
  }
  
  display.setCursor(0, 40);
  display.print("Bet: Rs.");
  if (bankroll < currentBet) {
    display.print(currentBet);
    display.print("!");
  } else {
    display.println(currentBet);
  }
  
  display.setCursor(0, 50);
  display.print("W:");
  display.print(longestWinStreak);
  display.print(" L:");
  display.print(longestLossStreak);
  
  display.print(" [");
  display.print(currentStrategy == MARTINGALE ? "M" : "F");
  display.print("]");
  
  if (bankroll > 0 && bankroll < currentBet * 2) {
    display.setCursor(0, 57);
    display.print("LOW FUNDS!");
  }
  
  display.display();
}

void switchStrategy() {
  if (currentStrategy == MARTINGALE) {
    currentStrategy = FLAT_BET;
  } else {
    currentStrategy = MARTINGALE;
  }
  
  currentBet = BASE_BET;
  
  tone(BUZZER, 1000, 100);
  delay(120);
  tone(BUZZER, 1200, 100);
  delay(120);
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 15);
  if (currentStrategy == MARTINGALE) {
    display.println("MARTIN");
    display.setCursor(10, 35);
    display.println("GALE");
  } else {
    display.println("FLAT");
    display.setCursor(10, 35);
    display.println("BET");
  }
  display.display();
  delay(1000);
  
  updateDisplay();
}

void resetGame() {
  bankroll = STARTING_BANKROLL;
  currentBet = BASE_BET;
  gamesPlayed = 0;
  gamesWon = 0;
  currentStreak = 0;
  longestWinStreak = 0;
  longestLossStreak = 0;
  lastGameWon = false;
  currentStrategy = MARTINGALE;
  
  tone(BUZZER, 800, 100);
  delay(120);
  tone(BUZZER, 1200, 100);
  delay(120);
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 25);
  display.println("RESET!");
  display.display();
  delay(1000);
  
  updateDisplay();
}

void handleBankruptcy() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 5);
  display.println("BANKRUPT");
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print("Games: ");
  display.println(gamesPlayed);
  display.setCursor(0, 40);
  display.print("Win Rate: ");
  display.print((float)gamesWon / gamesPlayed * 100, 1);
  display.println("%");
  display.setCursor(0, 55);
  display.println("Hold MODE 5s");
  display.display();
  
  for(int i = 0; i < 3; i++) {
    tone(BUZZER, 200, 300);
    delay(350);
  }
  
  while(digitalRead(BTN_MODE) == HIGH);
}
