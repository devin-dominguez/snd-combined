import React from 'react';

import Panel from './panel.jsx';

class DepthPanel extends React.Component {

  render() {
    return (
      <Panel
        device="depth"
        context={1}
        controls={[
          {name: 'left', options:[1, 5]},
          {name: 'right', options:[1, 5]},
          {name: 'top', options:[1, 5]},
          {name: 'bottom', options:[1, 5]},
          {name: 'rotation', options:[0.25, 1]},
          {name: 'near_clip', options:[10, 50]},
          {name: 'far_clip', options:[10, 50]},
        ]}
      />
    );
  }
}

export default DepthPanel;
