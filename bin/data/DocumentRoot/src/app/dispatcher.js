const storeCallbacks = [];

const Dispatcher = {
  addStore(storeCallback) {
    storeCallbacks.push(storeCallback);
  },

  dispatch(action) {
    storeCallbacks.forEach((storeCallback) => {storeCallback(action);});
  }
};

export default Dispatcher;

