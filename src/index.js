import React, { AppRegistry } from 'react-native'
import { Provider, connect } from 'react-redux'
import configureStore from './configureStore'

// components
import App from './containers/App'
import ChatList from './containers/ChatList'
import ChatContainer from './containers/ChatContainer'

import { Scene, Router } from 'react-native-router-flux'
global.isLoaded = false
const store = configureStore()

const RouterWithRedux = connect()(Router)

const xBand = () => (
  <Provider store = { store }>
    <RouterWithRedux>
			<Scene
				key = 'root'>
				<Scene
					key = "ChatList"
					component = { ChatList }
					title = "Chats"
					initial = { true }/>
				<Scene
					key = "ChatContainer"
					component = { ChatContainer }
					title = {
					"[friend name]"
				 	/*
				 		^ TODO:
				 		set from the message.from
				 	*/
				 }/>
			</Scene>
    </RouterWithRedux>
  </Provider>
)

AppRegistry.registerComponent('RNBoilerplate', () => xBand)
/*
	^ TODO:
	change 'RNBoilerplate' to 'xBand' ^^^ when
	using the android devices instead of simulator
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
	:* :* :* :* :* :* :* :* :* :* :* :* :* :* :* :*
*/
