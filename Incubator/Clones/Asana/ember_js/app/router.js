import EmberRouter from '@ember/routing/router';
import config from './config/environment';

//Based on my understanding, EmberRouter is a JavaScript engine which
// organizes and renders web pages through templates and components.
const Router = EmberRouter.extend({
  location: config.locationType,
  rootURL: config.rootURL
});

Router.map(function() {
  this.route('tasks');
  this.route('users');
});

export default Router;
