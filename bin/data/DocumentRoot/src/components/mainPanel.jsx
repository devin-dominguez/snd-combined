import React, { PropTypes, Component } from 'react'

import SndActions from '../app/sndActions.js';
import SndButton from './sndButton.jsx';

class _MainPanel extends Component {

  syncGui(e) {
    e.preventDefault();
    SndActions.syncGui();
  }


  render() {
    return (
        <div>
          <SndButton name="Sync GUI" callback={this.syncGui}/>
        </div>
    )
  }
}

_MainPanel.propTypes = {

}

export default _MainPanel
