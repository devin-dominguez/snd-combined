import React from 'react';

import Panel from './panel.jsx';
import SndButton from './sndButton.jsx';
import SndActions from '../app/sndActions.js';

class CvPanel extends React.Component {

  setCvPixels(e) {
    e.preventDefault();
    SndActions.changeCvPixels(0);
  }

  setRgbPixels(e) {
    e.preventDefault();
    SndActions.changeCvPixels(1);
  }

  render() {
    return (
      <div>
        <Panel
          device="rgb_calibration"
          context={2}
          controls={[
            {name: 'Brightness', options:[1, 5]},
            {name: 'Contrast', options:[1, 5]},
            {name: 'Exposure', options:[1, 5]},
            {name: 'Gain', options:[1, 5]},
            {name: 'Hue', options:[1, 5]},
            {name: 'Saturation', options:[1, 5]},
            {name: 'Sharpness', options:[1, 5]}
          ]}
        />
        <Panel
          device="cv"
          context={2}
          controls={[
            {name: 'blob_min_size', options:[1, 5]},
            {name: 'blob_max_size', options:[1, 5]},
            {name: 'num_blobs', options:[1]},
            {name: 'color_bottom', options:[1, 5]},
            {name: 'color_top', options:[1, 5]}
          ]}
        />
        <SndButton name="Cv Pixels" callback={this.setCvPixels}/>
        <SndButton name="RGB Pixels" callback={this.setRgbPixels}/>
      </div>

    );
  }
}

export default CvPanel;
