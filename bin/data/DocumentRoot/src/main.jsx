import React from 'react';
import {render} from 'react-dom';
import { Router, Route, Link, hashHistory } from 'react-router';

import Index from './components/index.jsx';
import DepthPanel from './components/depthPanel.jsx';
import RgbPanel from './components/rgbPanel.jsx';
import CvPanel from './components/cvPanel.jsx';
import Projector from './components/projector.jsx';

import SndStore from './app/sndStore.js';
import SndActions from './app/sndActions.js';

let token = SndStore.subscribe(() => {
  render((
    <Router history={hashHistory}>
      <Route path='/' component={Index}>
        <Route path='rgb' component={RgbPanel}/>
        <Route path='depth' component={DepthPanel}/>
        <Route path='cv' component={CvPanel}/>
        <Route path='projector' component={Projector}/>
      </Route>
    </Router>
  ), document.getElementById('app'));
  SndStore.unsubscribe(token);
});

window.onload = function() {
  SndActions.syncGui();
};
