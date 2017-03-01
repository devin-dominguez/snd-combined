#pragma once
#include "ofxOsc.h"
#include "entity.h"

class OSCEvent {
  public:
    OSCEvent(Entity::Type type, int voice, string param, double data);
    Entity::Type type;
    int voice;
    string param;
    double data;
};

class OSCSpecialEvent {
  public:
    OSCSpecialEvent(string type, vector<double> data);
    string type;
    vector<double> data;
};


class OSCHandler {
  public:
    static void setup(string address, int port);
    static void specialEvent(string type, vector<double> data);
    static void queueEvent(OSCEvent event);
    static void queueEvent(OSCSpecialEvent event);
    static void sendEvents();

  private:
    static vector<OSCEvent> events;
    static vector<OSCSpecialEvent> specialEvents;
    static ofxOscSender sender;
    static unordered_map<Entity::Type, string> entityNames;
};
