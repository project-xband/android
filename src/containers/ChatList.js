import React, { Component, PropTypes } from 'react-native'
import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';

import Chats from '../components/Chats';

import * as CounterActions from '../actions/counter';
import * as MessageActions from '../actions/message';

const mapStateToProps = state => {
   return {
      conversation: state.conversation,
   };
};

export default class ChatList extends Component {
	render() {
		return <Chats {...this.props}/>
	}
}

export default connect(mapStateToProps, mapDispatchToProps)(App);