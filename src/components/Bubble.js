import React, {
	Component,
	PropTypes,
	StyleSheet,
	Text,
	View
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

const Bubble = ({
	text,
	position,
}) =>
	<View 
    style = {[
        styles.bubble,
        (position === 'left' ? styles.bubbleLeft : styles.bubbleRight),
        ((text.length > 40) ? styles.flexy : null)
      ]}>
		<Text
      style = {[
          styles.text,
          (position === 'left' ? styles.textLeft : styles.textRight)
        ]}>
			{ text }
		</Text>
	</View>

Bubble.propTypes = {
	text: React.PropTypes.string.isRequired,
	position: React.PropTypes.oneOf(['left', 'right'])
}

Bubble.defaultProps = {
	...Component.defaultProps,
	text: 'Project xBand to the Moon!',
	position: 'left'
}

const styles = StyleSheet.create({
	flexy: { 
		// flex: 1 // not using flex now, now sure what's the issue...
	},
	bubble: {
  	borderRadius: 15,
  	paddingLeft: 14,
  	paddingRight: 14,
  	paddingBottom: 10,
  	paddingTop: 8
	},
	bubbleLeft: {
		marginRight: 70,
		marginLeft: 10,
		backgroundColor: '#e6e6eb',
   	alignSelf: 'flex-start'
 	},
	bubbleRight: {
  	marginLeft: 70,
  	marginRight: 10,
  	backgroundColor: '#007aff',
  	alignSelf: 'flex-end'
	},
	bubbleError: {
  	backgroundColor: '#e01717'
	},
	text: {
		fontSize: 18,
  	color: '#000'
	},
	textLeft: {
	},
	textRight: {
  	color: '#fff'
	}
})

export default Bubble