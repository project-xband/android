import React, { Component, PropTypes } from 'react-native'
import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';

import Bubble from '../components/Bubble';
import Chat from '../components/Chat';
import ChatCell from '../components/ChatCell';
import AnimatedTextInput from '../components/AnimatedTextInput';

import * as MessageActions from '../actions/message';
import * as ConversationActions from '../actions/conversation';

const mapStateToProps = state => ({
      messages: state.message,
      conversation: state.conversation
})

function mapDispatchToProps(dispatch) {
	return bindActionCreators(Object.assign({}, MessageActions, ConversationActions), dispatch)
}

export default connect(mapStateToProps, mapDispatchToProps)(Chat);