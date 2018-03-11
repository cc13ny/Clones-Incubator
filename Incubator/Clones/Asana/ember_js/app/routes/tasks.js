// import Route from '@ember/routing/route';

import Ember from 'ember';

let embr = Ember

embr.Component.extend({
  actions: {
    toggleBody() {
      this.toggleProperty('isShowingBody');
    }
  }
});

embr.Route.extend({
  model() {
    return ['Task 1: Requirement Analysis',
            'Task 2: Prioritize Requirements',
            'Task 3: Design API'];
  }
}

export default embr;
