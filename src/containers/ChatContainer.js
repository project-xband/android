import React, {
	Component,
	PropTypes
} from 'react-native'

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

import Bubble from '../components/Bubble'
import Chat from '../components/Chat'
import ChatCell from '../components/ChatCell'
import AnimatedTextInput from '../components/AnimatedTextInput'

import * as MessageActions from '../actions/message'
import * as ConversationActions from '../actions/conversation'

const mapStateToProps = state => ({
  messages: state.message,
  conversation: state.conversation,
  device: state.device
})

function mapDispatchToProps(dispatch) {
	return bindActionCreators(Object.assign({}, MessageActions, ConversationActions), dispatch)
}

export default connect(mapStateToProps, mapDispatchToProps)(Chat)
