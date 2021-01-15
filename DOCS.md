# Documentation 

<!-- TABLE OF CONTENTS -->
## Table of Contents

* [Config](#config)
* [Runner](#runner)
* [FrontendAction](#frontendaction)
* [ITransformation](#itransformation)
* [ASTConsumer](#astconsumer)
* [ASTVisitor](#astvisitor)


<!-- CONFIG -->
## Config

Config is a `.yaml` file having the following structure:
```
n transformations: 2
batch size: 16
transformations:
  - identity transform:
      p: 0.2
  - remove comments:
      p: 0.9
  - add comments:
      p: 0.2
      statements: ["forInside", "ifBegin", "ifInside"]
  ...
```

Applying transformations is a randomized process. Therefore, each transformation has an attribute
`p` which determines the probability of certain transformation to be applied. 

- `transformations` -- a fixed set of transformations
- `batch size` -- files from `input path` are processed by batches which size is determined by this argument
- `n transformations` determines how many times transformations should be applied to each file. 
Since transformations is applied with some probability `p`, running the same transformations multiple 
times will produce different transformed code.



<!-- RUNNER -->
## Runner

Runner is a class that consumes a set of transformations, constructs a 
`ClangTool` instance. Then the `ClangTool` runs `n transformations` times on 
all files passed through `argv`.



<!-- FRONTEND ACTION -->
## FrontendAction

`FrontendAction` is an interface to run `ASTConsumer`.

<!-- ITRANSFORMATION -->
### ITransformation

Base class for transformations, which performs storing data about transformation (e.g probability `p`) and 
creating `ASTConsumer` instances.


<!-- ASTCONSUMER -->
### ASTConsumer

`ASTConsumer` is an interface for interacting with AST, this abstraction layer allows
to be independent of the AST producer. `ASTConsumer` calls method `HandleTranslationUnit` for every
`ASTContext` instance obtained from AST. Then `RecursiveASTVisitor` goes through top declaration context 
which is obtained from `ASTContext`, collects information about transformation in `RewriteBuffer`. Finally, 
the transformations are applied and saved in `ASTConsumer` using `overwriteChangedFiles` method. to For more detailed 
information go to [clang doc](https://clang.llvm.org/doxygen/classclang_1_1ASTConsumer.html)


<!-- ASTVISITOR -->
### ASTVisitor

`RecursiveASTVisitor` is a set of actions that are done when a certain node of AST is reached. For more detailed 
information go to [clang doc](https://clang.llvm.org/doxygen/classclang_1_1RecursiveASTVisitor.html)
