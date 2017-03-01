#include "gc_osc.h"

/////////////////////////////////////////
// OSC EVENTS
/////////////////////////////////////////
OSCEvent::OSCEvent(Entity::Type type, int voice, string param, double data) {
  this->type = type;
  this->voice = voice;
  this->param = param;
  this->data = data;
}

OSCSpecialEvent::OSCSpecialEvent(string type, vector<double> data) {
  this->type = type;
  this->data = data;
}


/////////////////////////////////////////
// OSC HANDLER
/////////////////////////////////////////
vector <OSCEvent> OSCHandler::events;
vector <OSCSpecialEvent> OSCHandler::specialEvents;
ofxOscSender OSCHandler::sender;
unordered_map<Entity::Type, string> OSCHandler::entityNames;

void OSCHandler::setup(string address, int port) {
  sender.setup(address, port);
  entityNames[Entity::DEFAULT] = "default";
  entityNames[Entity::PULSAR] = "pulsar";
  entityNames[Entity::PULSE] = "pulse";
  entityNames[Entity::NODE] = "node";
  entityNames[Entity::ROCKET] = "rocket";
  entityNames[Entity::BLIMP] = "blimp";
}

void OSCHandler::specialEvent(string type, vector<double> data) {
  queueEvent(OSCSpecialEvent(type, data));
}

void OSCHandler::queueEvent(OSCEvent event) {
  events.push_back(event);
}

void OSCHandler::queueEvent(OSCSpecialEvent event) {
  specialEvents.push_back(event);
}

void OSCHandler::sendEvents() {
  for (size_t i = 0; i < events.size(); i++) {
    ofxOscMessage message;
    string entity = entityNames[events[i].type];
    message.setAddress("/" + entity);
    message.addIntArg(events[i].voice);
    message.addStringArg(events[i].param);
    message.addFloatArg(events[i].data);
    sender.sendMessage(message);
  }
  for (size_t i = 0; i < specialEvents.size(); i++) {
    ofxOscMessage message;
    message.setAddress("/" + specialEvents[i].type);
    for (size_t d = 0; d < specialEvents[i].data.size(); d++) {
      message.addFloatArg(specialEvents[i].data[d]);
    }
    sender.sendMessage(message);
  }

  events.clear();
  specialEvents.clear();
}
