import React, { Component } from 'react-native'
import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';

import Bubble from '../components/Bubble';
import Chat from '../components/Chat';
import ChatCell from '../components/ChatCell';
import AnimatedTextInput from '../components/AnimatedTextInput';

import * as CounterActions from '../actions/counter';
import * as MessageActions from '../actions/message';

const mapStateToProps = state => state.messages;
const mapDispatchToProps = dispatch => bindActionCreators(MessageActions, dispatch);

@connect(mapStateToProps, mapDispatchToProps)(App)
export default class App extends Component {
	render() {
		return <Chat {...this.props}/>
	}
}