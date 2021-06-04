import React from 'react'
import { Button, Modal } from 'semantic-ui-react'

/**
 * 
 * help modal for app
 */
function Help() {
  const [open, setOpen] = React.useState(false)

  return (
    <Modal
      onClose={() => setOpen(false)}
      onOpen={() => setOpen(true)}
      open={open}
      trigger={<Button color='blue' >Help</Button>}
    >
      <Modal.Header>Help</Modal.Header>
      <Modal.Content>

      <h2>What is being displayed</h2>

        <p> On both sides, the entries of a 32 by 32 matrix are displayed in row major order. The black square is the current entry being accessed in the array. The grayed squares are the entries that reside in a cache with width 8 and height 8 using a LRU policy. There are three different indexing designs that can be chosen: row-major indexing, z indexing, and hilbert indexing. Row-major indexing stores the entries of the array behind the scenes in a row major order, z indexing does so using a z-order curve, and Hilbert indexing uses a pseudo Hilbert curve. Finally, below each array, some basic statistics regarding cache hits and misses are shown.</p>

      <h2>Usage</h2>
      <ul>
          <li>Use the dropdowns on the two sides of the page to select the type of indexing </li>
          <li>Use the teleport UI in the center to teleport the pointer of all matrices to any entry of the array</li>
          <li>Use WASD to move the pointer through the array</li>
        </ul>

      </Modal.Content>
      <Modal.Actions>
        <Button color='blue' onClick={() => setOpen(false)}>
          Close
        </Button>
      </Modal.Actions>
    </Modal>
  )
}

export default Help