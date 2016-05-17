import React, { Component, PropTypes } from 'react-native'

/*
  ^ there is a new format they said (new RN 0.25.1) 
  where the import is like -->

  ````
    import React, { Component } from 'react'
    import { 
      PropTypes,
      StyleSheet,
      Dimensions,
      Text,
      TextInput,
      View,
      TouchableHighlight
    } from 'react-native'
  ````

  (https://github.com/facebook/react-native/releases/tag/v0.25.1)
*/

import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { Router } from 'react-native-router-flux'

import Chat from '../components/Chat'
import * as MessageActions from '../actions/message'

const RouterWithRedux = connect()(Router)

const mapStateToProps = state => {
   return {
      messages: state.message,
      conversation: state.conversation
   }
}
const mapDispatchToProps = dispatch => bindActionCreators(MessageActions, dispatch)

class App extends Component {
	render() {
		return (
			this.props.children
		)
	}
}

export default connect(mapStateToProps, mapDispatchToProps)(App)