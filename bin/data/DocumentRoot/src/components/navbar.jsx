import React, { PropTypes, Component } from 'react'

import SndButton from './sndButton.jsx';

class _NavBar extends Component {

  render() {
    return (
      <header>
        <ul>
          <li><a href="#/rgb">Rgb</a></li>
          <li><a href="#/depth">Depth</a></li>
          <li><a href="#/cv">Cv</a></li>
          <li><a href="#/projector">Projector</a></li>
        </ul>
      </header>
    )
  }
}

_NavBar.propTypes = {

}

export default _NavBar
