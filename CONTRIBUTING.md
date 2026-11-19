# Contributing

Thanks for your interest! This is a small educational project, so contributions are welcome
as long as they keep it simple and readable.

## How to contribute

1. Open an issue describing the bug or idea before starting larger changes.
2. Fork the repository and create a branch (`fix/...`, `feat/...`, `docs/...`).
3. Build and run everything locally before opening a pull request:

   ```bash
   make && make test && make smoke
   ```

4. Use [Conventional Commits](https://www.conventionalcommits.org/) in English
   (`feat:`, `fix:`, `docs:`, `test:`, `ci:`, `refactor:`, `chore:`).
5. Open a pull request explaining what changed and why. CI must be green.

## Guiding principles

- Keep the code beginner-friendly: plain C11, no external libraries.
- Keep cipher logic in `src/cipher.c` free of I/O so it stays testable.
- Do not edit `original/` — it is kept as the historical course submission.

## Privacy

Never commit real messages, `cipher.txt` or `key.txt`. They are ignored by `.gitignore`.
