import React, { PropTypes, Component } from 'react'

class _SndButton extends Component {

  render() {
    return (
      <button onClick={this.props.callback}>
        {this.props.name}
      </ button>
    )
  }
}

_SndButton.propTypes = {

}

export default _SndButton
