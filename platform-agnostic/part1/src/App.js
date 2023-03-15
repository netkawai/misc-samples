import logo from './logo.svg';
import './App.css';

const Hello = (props) => {
  return(
    <div>
      <p>Another Hello World from {props.name}</p>
    </div>
  )
}

const App = () => {
  const now = new Date()
  const a = 10
  const b = 20

  console.log('Hello from component')
  return (
    <div>
      <p>Hello World, it is {now.toString()}</p>
      <p>
        {a} plus {b} is {a+b}
      </p>
      <Hello name='Kawai' />
    </div>
  )
}

export default App;
