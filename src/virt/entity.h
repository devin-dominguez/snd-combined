#pragma once

#include "ofMain.h"

class Entity {
  public:
    ////////////////////////////
    // STATIC METHODS //////////
    ////////////////////////////
    template<class Collection>
    static void updateCollection(Collection &collection, double dt);

    template<class Collection>
    static void drawCollection(Collection &collection);

    template<class Collection>
    static void pruneCollection(Collection &collection);

    template<class CollectionA, class CollectionB>
    static void collideCollection(
      CollectionA &collectionA,
      CollectionB &collectionB
    );

    static bool areColliding(Entity &entityA, Entity &entityB);
    ////////////////////////////
    // CONSTRUCTOR /////////////
    ////////////////////////////
    Entity(ofPoint position);

    enum Type {
      DEFAULT,
      PULSAR,
      PULSE,
      NODE,
      ROCKET,
      BLIMP
    } type;

    void kill();
    void death();

    ////////////////////////////
    // GETTERS /////////////////
    ////////////////////////////
    ofPoint getPosition();
    double getSize();
    bool isDead();
    bool isCollidable();

    static ofFloatColor colors[5];

  protected:
    int voice;
    void oscEvent(string param, double data);
    ////////////////////////////
    // PROPERTIES //////////////
    ////////////////////////////
    ofPoint position;
    double size;
    bool collidable;
    bool colliding;

    // time in ms
    double fadeInTime;
    double fadeOutTime;
    // 0.0 < n <= 1.0
    double fadeLevel;

    ////////////////////////////
    // METHODS /////////////////
    ////////////////////////////
    void fadeIn(double dt);
    void fadeOut(double dt);
    void collideWith(Entity* entity);

    ////////////////////////////
    // ENUMS ///////////////////
    ////////////////////////////
    enum Phase {
      NEW,
      ALIVE,
      DYING,
      DEAD
    } phase;
};
