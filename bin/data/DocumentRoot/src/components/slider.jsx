import React, { PropTypes, Component } from 'react'

import SndStore from '../app/sndStore.js';
import SndActions from '../app/sndActions.js';

class _Slider extends Component {
  constructor(props) {
    super(props);

    let sndState = SndStore.getState();
    this.state = {
      value: sndState[this.props.device][this.props.param]
    }

    this.update = this.update.bind(this);
    this.onChange = this.onChange.bind(this);
  }

  componentWillMount() {
    this.token = SndStore.subscribe(this.update);
  }

  componentWillUnmount() {
    SndStore.unsubscribe(this.token);
  }

  update() {
    let sndState = SndStore.getState();
    this.setState({
      value: sndState[this.props.device][this.props.param]
    });
  }

  onChange(e) {
    e.preventDefault();

    let newValue = Number(e.target.value);
    SndActions.setValueFromSlider({
      device: this.props.device,
      param: this.props.param,
      value: newValue
    });
  }

  render() {
    return (
      <input
        type="range"
        min={this.props.minValue}
        max={this.props.maxValue}
        value={this.state.value}
        onChange={this.onChange}
      />
    )
  }
}

_Slider.propTypes = {

}

export default _Slider
