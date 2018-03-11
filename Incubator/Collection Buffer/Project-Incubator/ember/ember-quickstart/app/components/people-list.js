import Ember from 'ember';

export default Ember.Component.extend({
  actions: {
    showPage(person) {
      console.log(person);
    }
  }
});
