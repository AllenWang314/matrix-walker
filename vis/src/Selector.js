import React from "react";
import { Dropdown } from 'semantic-ui-react'
import 'semantic-ui-css/semantic.min.css'

const options = [{
  key: 0,
  text: 'Row-major Indexing',
  value: 0,
},
{
  key: 1,
  text: 'Z Indexing',
  value: 1,
},
{
  key: 2,
  text: 'Hilbert Indexing',
  value: 2,
}]

/**
 * dropdown selector for matrix selection
 * @param {*} props 
 */
const Selector = (props) => {
  return <Dropdown 
    placeholder='Select Matrix' 
    selection
    options={options} 
    onChange={(_, data) => {props.selected(data.value);}}/>
}

export default Selector