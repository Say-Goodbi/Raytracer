# Commitlint Configuration

## Purpose

### What Commitlint Does

Commitlint is a tool that validates your commit messages against a set of well-defined rules. It ensures that all commits in your project follow a consistent format and convention, making your repository's history more readable, organized, and maintainable.

In this project, commitlint is configured with the **Conventional Commits** standard, which is a specification for adding human and machine readable meaning to commit messages.

### Why It's Useful

1. **Consistency**: Enforces a standardized format across all commits, making the commit history uniform and professional
2. **Semantic Versioning**: Works seamlessly with semantic-release to automatically determine version bumps (major, minor, patch) based on commit types
3. **Automated Changelog Generation**: Commit message format is parsed to automatically generate changelogs
4. **Better Code Review**: Clear commit messages make it easier to understand what changes were made and why
5. **Team Standards**: Helps teams maintain code quality standards and best practices
6. **Integration with CI/CD**: Prevents poorly formatted commits from entering the repository via pre-commit hooks

---

## How to Use It

### Setup

To set up commitlint in your project, execute the setup script from the `dev` directory:

```bash
cd dev
bash setup-commitlint.sh
```

This script automatically:

1. **Installs Dependencies** - Installs the following packages as dev dependencies:
   - `@commitlint/cli` (v20.5.0) - Command line tool for linting commit messages
   - `@commitlint/config-conventional` (v20.5.0) - Conventional Commits configuration and rules
   - `husky` (v9.1.7) - Git hooks manager that automatically runs commitlint on commits

2. **Initializes Husky** - Sets up Husky in the project to manage git hooks

3. **Creates Git Hook** - Adds a `commit-msg` hook that:
   - Runs automatically when you execute `git commit`
   - Validates the commit message against the Conventional Commits rules
   - Prevents commits that don't follow the format
   - Shows error messages if validation fails

4. **Creates Configuration File** - Generates `commitlint.config.js` with:
   ```javascript
   module.exports = { extends: ['@commitlint/config-conventional'] };
   ```
   This configuration extends the standard Conventional Commits rules.

### After Setup

Once the setup is complete, commitlint runs automatically on every `git commit`. You don't need to do anything special—just write your commit message following the Conventional Commits format. If your message doesn't follow the rules, the commit will be rejected and you'll see an error message explaining what's wrong.

### Conventional Commit Format

A conventional commit has the following structure:

```
<type>(<scope>): <subject>

<body>

<footer>
```

#### Type (Required)
Must be one of:
- **feat**: A new feature
- **fix**: A bug fix
- **docs**: Documentation only changes
- **style**: Changes that don't affect code meaning (formatting, semicolons, etc.)
- **refactor**: Code changes that neither fix a bug nor add a feature
- **perf**: Code changes that improve performance
- **test**: Adding or updating tests
- **chore**: Changes to build process, dependencies, or tooling
- **ci**: Changes to CI/CD configuration

#### Scope (Optional)
A noun describing the part of the codebase affected, e.g., `api`, `auth`, `ui`.

#### Subject (Required)
- Use imperative mood ("add" not "added" or "adds")
- Don't capitalize the first letter
- No period at the end
- Maximum 50 characters

#### Body (Optional)
- Explain the motivation for the change
- Explain what the change does and why, not how
- Use imperative mood
- Separate from subject with a blank line
- Wrap at 72 characters

#### Footer (Optional)
- Reference issues: `Closes #123`
- Note breaking changes: `BREAKING CHANGE: description`

### Examples

#### ✅ Commits That Pass Validation

```
feat(auth): add JWT token refresh mechanism
```

```
fix(api): handle null values in response parser

This prevents a crash when the API returns incomplete data.
The parser now gracefully skips null fields while logging a warning.

Closes #456
```

```
docs(readme): update installation instructions
```

```
refactor(core): simplify event handler logic
```

```
test(utils): add edge case tests for string validation
```

#### ❌ Commits That Fail Validation

```
Added new login feature
```
❌ Missing type and scope. Should be: `feat(auth): add login feature`

```
fixed bug
```
❌ Missing scope and too vague. Should be: `fix(api): handle null reference error`

```
FEAT: Update authentication
```
❌ Type must be lowercase. Should be: `feat: update authentication`

```
feat: Add new feature (50 characters max is common best practice)
```
❌ Subject too long and not following imperative mood.

```
feat(auth): Fix the login
```
❌ Not using imperative mood. Should be: `feat(auth): update login validation`

### Testing Commitlint Locally

To test commitlint without committing:

```bash
echo "feat(api): add new endpoint" | npx commitlint
echo "invalid message" | npx commitlint  # This will fail
```

### Bypassing Commitlint (Not Recommended)

In rare cases, you can skip the pre-commit hook:

```bash
git commit --no-verify -m "your message"
```

⚠️ **Warning**: This should only be used in exceptional circumstances, as it defeats the purpose of enforcing standards.

### Integration with Semantic Release

Commitlint works with semantic-release to automatically:
- Detect `feat:` commits as minor version bumps
- Detect `fix:` commits as patch version bumps
- Detect `BREAKING CHANGE:` as major version bumps
- Generate changelogs with organized sections

This automation requires commits to be properly formatted with commitlint-compatible messages.

---

## What's Used

This project uses the following tools and packages:
- **@commitlint/cli** (v20.5.0) - Command line tool for linting commits
- **@commitlint/config-conventional** (v20.5.0) - Conventional commits configuration
- **husky** (v9.1.7) - Git hooks manager that runs commitlint on commit-msg
- **semantic-release** (v25.0.3) - Automated versioning and changelog generation based on commits