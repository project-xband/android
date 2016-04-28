import React, { Component, PropTypes } from 'react-native'
import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';

import Bubble from '../components/Bubble';
import Chat from '../components/Chat';
import ChatCell from '../components/ChatCell';
import AnimatedTextInput from '../components/AnimatedTextInput';

import * as MessageActions from '../actions/message';

const mapStateToProps = state => {
   return {
      messages: state.message,
      conversation: state.conversation
   };
};

const mapDispatchToProps = dispatch => bindActionCreators(MessageActions, dispatch);

class ChatContainer extends Component {
	render() {
		console.log('chat cont props',this.props)
		return <Chat {...this.props}/>
	}
}

export default connect(mapStateToProps, mapDispatchToProps)(ChatContainer);