import React, { PropTypes, Component } from 'react'

import SndStore from '../app/sndStore.js';
import SndActions from '../app/sndActions.js';

class _Bumpers extends Component {
  makeButtons() {
    let inverted = this.props.options.map((val) => val * -1);
    let combined = inverted.reverse().concat(this.props.options);
    return combined.map((val, idx) => (
      <button
        key={idx}
        value={val}
        onClick={this.props.callback}
      >
        {val > 0 ? `+${val}` : val}
      </button>
    ));
  }

  render() {
    return (
      <div>
        {this.makeButtons()}
      </div>
    )
  }
}

export default _Bumpers
