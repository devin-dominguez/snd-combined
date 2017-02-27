import React, { PropTypes, Component } from 'react'

import SndStore from '../app/sndStore.js';
import SndActions from '../app/sndActions.js';
import Bumpers from './bumpers.jsx';

class _Control extends Component {
  constructor(props) {
    super(props);

    let sndState = SndStore.getState();
    let param = sndState[this.props.device][this.props.param];
    this.state = {
      value: param.value
    }

    this.min = Number(param.min);
    this.max = Number(param.max);

    this.update = this.update.bind(this);
    this.onSliderChange = this.onSliderChange.bind(this);
    this.onBumperClick = this.onBumperClick.bind(this);
  }

  componentWillMount() {
    this.token = SndStore.subscribe(this.update);
  }

  componentWillUnmount() {
    SndStore.unsubscribe(this.token);
  }

  update() {
    let sndState = SndStore.getState();
    let param = sndState[this.props.device][this.props.param];
    this.setState({
      value: param.value
    });
  }

  onSliderChange(e) {
    e.preventDefault();

    let newValue = Number(e.target.value);
    SndActions.setParam({
      device: this.props.device,
      param: this.props.param,
      value: newValue
    });
  }

  onBumperClick(e) {
    let newValue = Number(e.target.value) + Number(this.state.value);
    newValue = Math.min(this.max, Math.max(this.min, newValue));
    SndActions.setParam({
      device: this.props.device,
      param: this.props.param,
      value: newValue
    });
  }

  render() {
    return (
      <div>
        <span>{`${this.props.device}.${this.props.param}`}</span>
        <span>{this.state.value}</span>
        <input
          type="range"
          min={this.min}
          max={this.max}
          value={this.state.value}
          onChange={this.onSliderChange}
        />
        <Bumpers
          options={this.props.options}
          callback={this.onBumperClick}
        />
      </div>
    )
  }
}

_Control.propTypes = {

}

export default _Control
