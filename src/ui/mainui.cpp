#include "base.h"
#include "mainui.h"
#include "utils.h"
#include "../globals.h"

#include <thread>

void MainUI::display(ui::Screen::ptr screen) {
  currentScreen->exit();
  currentScreen = screen;
  currentScreen->init();
}

void MainUI::init() {
  uiThread = std::thread([this](){mainRoutine();});
}

void MainUI::exit() {
  uiThread.join();
}

void MainUI::draw(drawing::Canvas& ctx) {
  currentScreen->draw(ctx);
}

bool MainUI::keypress(ui::Key key) {
  keys[key] = true;
  if (globKeyPre(key)) return true;
  if (currentScreen->keypress(key)) return true;
  return globKeyPost(key);
}

bool MainUI::keyrelease(ui::Key key) {
  keys[key] = false;
  return currentScreen->keyrelease(key);
}

bool MainUI::globKeyPost(ui::Key key) {
  switch (key) {
  case ui::K_PLAY:
    if (GLOB.tapedeck.state.playing()) {
      GLOB.tapedeck.state.stop();
    } else {
      GLOB.tapedeck.state.play();
    }
    return true;
  }
  return false;
}

bool MainUI::globKeyPre(ui::Key key) {
  using namespace ui;
  switch (key) {
  case K_QUIT:
    GLOB.exit();
    break;
  case K_TAPE:
    GLOB.tapedeck.display();
    break;
  case K_MIXER:
    GLOB.mixer.display();
    break;
  case K_SYNTH:
    GLOB.synth.display();
    break;
  case K_METRONOME:
    GLOB.metronome.display();
    break;
  default:
    return false;
  }
  return true;
}
