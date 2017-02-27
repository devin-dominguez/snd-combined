class Store {
  constructor(dispatcher) {
    dispatcher.addStore(this.storeCallback.bind(this));
    this.subToken = 0;
    this.subs = {};
  }

  notifySubs() {
    Object.keys(this.subs).forEach((key) => this.subs[key]())
  }

  storeCallback(action) {
    this.receiveAction(action);
  }

  subscribe(subCallback) {
    this.subs[++this.subToken] = subCallback;
    return this.subToken;
  }

  unsubscribe(token) {
    delete this.subs[token];
  }
}

export default Store;
