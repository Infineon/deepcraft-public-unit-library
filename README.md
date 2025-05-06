# Deepcraft™ Studio public unit library

DEEPCRAFT™ Studio (formerly Imagimob Studio) is an end-to-end platform for developing AI / Machine Learning (ML) models for the edge. The platform is designed to support users in creating robust and high-quality models that are ready for deployment in commercial products. 

This repository is automatically pulled and content is generated in DEEPCRAFT™ Studio. For the best experience, access these units through DEEPCRAFT™ Studio.

For commercial use, our standard terms and conditions applies, https://developer.imagimob.com/legal/studio-terms-and-conditions.

## Usage
These units are designed to be used through DEEPCRAFT™ Studio (https://www.imagimob.com/studio) and should be accessed through that platform.

## Contribution
All users are welcome to submit new units, subject to the Infineon DEEPCRAFT™ unit review process.

## Submission Process
To submit a unit, create a pull request using the automation tool provided below.

All unit submissions must include a README file with a use-case description.

Once the unit is ready, you can download pr_tool.exe and run:

```
pr_tool.exe
cd pr_tool
python .\pr_tool.py --path <unit-path>
```

where <unit-path> is the root path of the unit. For more information review the tools' README.md file.

Please be aware that you will need a GitHub Account. When you run the tool using the command shown above it will authenticate using your GitHub account, fork this repository and prepare the pull request. Once ready, it will open the pull request in a window in your browser. Please add the relevant detail requested to complete your pull request which will aid in the review process and then submit.
