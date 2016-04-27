import React, { Component, PropTypes } from 'react-native'
import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';

import Bubble from '../components/Bubble';
import Chat from '../components/Chat';
import ChatCell from '../components/ChatCell';
import AnimatedTextInput from '../components/AnimatedTextInput';

import * as CounterActions from '../actions/counter';
import * as MessageActions from '../actions/message';

const mapStateToProps = state => {
   return {
      messages: state.messages
   };
};
const mapDispatchToProps = dispatch => bindActionCreators(MessageActions, dispatch);

export default class App extends Component {
	render() {
		return <Chat {...this.props}/>
	}
}

export default connect(mapStateToProps, mapDispatchToProps)(App);