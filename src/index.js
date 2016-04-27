import React, { AppRegistry } from 'react-native';
import { Provider } from 'react-redux';
import App from './containers/App';
import configureStore from './configureStore';

const store = configureStore();

// TODO: add the Router here - How does it interplay with Redux? :C
const xBand = () => (
  <Provider store={store}>
    <App />
  </Provider>
);

AppRegistry.registerComponent('RNBoilerplate', () => xBand);
//TODO: change 'RNBoilerplate' to 'xBand' ^^^