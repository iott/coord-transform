{
  "targets": [{
    "target_name": "transform",
    "sources": [
      "coordtransform.cc"
    ],
    "include_dirs": [
      "<!(node -e \"require('nan')\")"
    ]
  }]
}