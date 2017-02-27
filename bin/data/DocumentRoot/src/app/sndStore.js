import Store from './store.js';
import Dispatcher from './dispatcher.js';

let state = {};

const SndStore = new Store(Dispatcher);
SndStore.receiveAction = function(action) {
  let payload = action.payload;
  switch (action.type) {
    case "SYNC_GUI":
      state = payload;
      this.notifySubs();
      break;
    case "SET_PARAM":
      state[payload.device][payload.param].value = payload.value;
      this.notifySubs();
      break;
    case "SAVE":
    case "RESET":
      this.notifySubs();
      break;
  }
}

window.store = SndStore;
SndStore.getState = function() {
  return Object.assign({}, state);
}

export default SndStore;
