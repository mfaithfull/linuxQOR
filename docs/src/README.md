# This folder, docs/src, is the root of the low level code documentation tree

## Structure
Scoped folders of markdown files that parallel the stucture of the code under the workspace src folder
Documentation for src/platform/os/windows is found under docs/src/platform/os/windows
Documentation for a specific TU should be in a file matching it's name. To find docs specific to src/qor/flyers/error/errorbase.* look at docs/src/qor/flyers/error/errorbase.md

## Content
These documents are to express intent, design, usage and operational notes for class and function level constructs. Also to express ideas, guidance and understanding for specific, non module, scopes within the source tree such as Linux specific documentation within docs/src/platform/os/linux and items that apply to the entire src/framework subtree within docs/src/framework.

## Restrictions
These documents must not contain code metrics. Line numbers, line counts and function counts are strictly forbidden. If the addition of a function to the documented file can invalidate a statement then that statement does not belong in these documents. No measurements, only explanation.