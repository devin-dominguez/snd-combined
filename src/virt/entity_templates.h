#include "entity.h"

template<class Collection>
void Entity::updateCollection(Collection &collection, double dt) {
  for (unsigned int i = 0; i < collection.size(); ++i) {
    collection[i].update(dt);
  }
}

template<class Collection>
void Entity::drawCollection(Collection &collection) {
  for (unsigned int i = 0; i < collection.size(); ++i) {
    collection[i].draw();
  }
}

template<class Collection>
void Entity::pruneCollection(Collection &collection) {
  collection.erase(
    remove_if(
      collection.begin(),
      collection.end(),
      [](Entity& entity) { return entity.isDead(); }
    ),
    collection.end()
  );
}

template<class CollectionA, class CollectionB>
void Entity::collideCollection(
  CollectionA &collectionA,
  CollectionB &collectionB
  ) {
  for (unsigned int a = 0; a < collectionA.size(); ++a) {
    for (unsigned int b = 0; b < collectionB.size(); ++b) {
      if (areColliding(collectionA[a], collectionB[b])) {
        collectionA[a].collideWith(&collectionB[b]);
        collectionB[b].collideWith(&collectionA[a]);
      }
    }
  }
}
