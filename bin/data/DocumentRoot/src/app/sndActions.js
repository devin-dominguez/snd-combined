import Dispatcher from './dispatcher.js';
import $ from 'jquery';

const ws = new WebSocket('ws://192.168.12.1:8197');
const wsRpc = function(device, param, value) {
  const msg = {
    jsonrpc: '2.0',
    method: device,
    params: [param, value],
    id: device
  };

  ws.send(JSON.stringify(msg));
}

const postRpc = function(device, param, value) {
  const data = {
    jsonrpc: '2.0',
    method: device,
    params: [param, value],
    id: device
  };
  return $.ajax({
    url: '/post',
    method: 'POST',
    contentType: "application/json",
    data: JSON.stringify(data)
  });
}

const SndActions = {
  syncGui() {
    postRpc('server', 'sync').then((res) => {
      let action = {
        type: "SYNC_GUI",
        payload: JSON.parse(res).result
      };
      console.log(action);
      Dispatcher.dispatch(action);
    });
  },

  changeCvPixels(pixels) {
    postRpc('server', 'cv_pixels', pixels);
  },

  resetParams(device) {
    postRpc(device, 'reset').then((res) => {
      let action = {
        type: "RESET"
      };
      Dispatcher.dispatch(action);
      this.syncGui();
    })
  },

  setVideoContext(context) {
    postRpc('server', 'context', context);
  },

  saveParams(device) {
    postRpc(device, 'save').then((res) => {
      let action = {
        type: "SAVE"
      };
      Dispatcher.dispatch(action);
    });
  },

  loadParams(device) {
    postRpc(device, 'load').then((res) => {
      let action = {
        type: "LOAD"
      };
      Dispatcher.dispatch(action);
      this.syncGui();
    });
  },

  setParam(params) {
    let action = {
      type: "SET_PARAM",
      payload: params
    }

    wsRpc(params.device, params.param, params.value);
    Dispatcher.dispatch(action);
  },
};

export default SndActions;
