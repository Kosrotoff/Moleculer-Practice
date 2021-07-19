{
  "targets": [
    {
      "target_name": "FibonacciSequence",
      "sources": [ "src/addons/FibonacciSequence/FibonacciSequence.cpp" ],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}