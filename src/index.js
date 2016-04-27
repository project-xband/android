import React, { AppRegistry } from 'react-native';
import { Provider, connect } from 'react-redux';
import App from './containers/App';
import ChatList from './containers/ChatList';
import configureStore from './configureStore';

import { Scene, Router } from 'react-native-router-flux';

const store = configureStore();

const RouterWithRedux = connect()(Router);

// TODO: add the Router here - How does it interplay with Redux? :C
const xBand = () => (
  <Provider store={store}>
    <RouterWithRedux>
		<Scene key='root'>
			<Scene key="App" component={App} title="Chat"/>
		</Scene>
    </RouterWithRedux>
  </Provider>
);

AppRegistry.registerComponent('RNBoilerplate', () => xBand);
//TODO: change 'RNBoilerplate' to 'xBand' ^^^