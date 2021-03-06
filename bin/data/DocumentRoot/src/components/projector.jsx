import React from 'react';

import Panel from './panel.jsx';

class ProjectorPanel extends React.Component {

  render() {
    return (
      <Panel
        device="projector"
        context={3}
        controls={[
          {name: 'left', options:[1, 5]},
          {name: 'right', options:[1, 5]},
          {name: 'top', options:[1, 5]},
          {name: 'bottom', options:[1, 5]},
          {name: 'rotation', options:[0.25, 1]},
          {name: 'x', options:[1, 5]},
          {name: 'y', options:[0.25, 1]},
        ]}
      />
    );
  }
}

export default ProjectorPanel;
