import React from 'react';

import Control from './control.jsx';
import SndButton from './sndButton.jsx';
import SndActions from '../app/sndActions.js';

class Panel extends React.Component {
  constructor(props) {
    super(props);

    this.loadParams = this.loadParams.bind(this);
    this.saveParams = this.saveParams.bind(this);
    this.reset = this.reset.bind(this);
  }

  componentWillMount() {
    SndActions.setVideoContext(this.props.context);
  }

  loadParams(e) {
    e.preventDefault();
    SndActions.loadParams(this.props.device);
  }

  saveParams(e) {
    e.preventDefault();
    SndActions.saveParams(this.props.device);
  }

  reset(e) {
    e.preventDefault();
    SndActions.resetParams(this.props.device);
  }

  getControls() {
    return this.props.controls.map((param, idx)=> {
      return (
        <Control
          key={idx}
          device={this.props.device}
          param={param.name}
          options={param.options}
        />
      );
    });
  }

  render() {
    return (
      <div>
        <SndButton name="Save" callback={this.saveParams}/>
        <SndButton name="Load" callback={this.loadParams}/>
        <SndButton name="Reset" callback={this.reset}/>
        {this.getControls()}
      </div>
    );
  }
}

export default Panel;

