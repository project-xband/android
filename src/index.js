import React, { AppRegistry } from 'react-native';
import { Provider } from 'react-redux';
import App from './containers/App';
import configureStore from './configureStore';

const store = configureStore();

const xBand = () => (
  <Provider store={store}>
    <App />
  </Provider>
);

AppRegistry.registerComponent('xBand', () => xBand);
