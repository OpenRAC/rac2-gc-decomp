#!/usr/bin/env python3
"""Analyze and rank functions by size across the RAC2 codebase.

This script extracts function sizes from the compiled ELF and generates a ranked
list that can be used to identify small, medium, and large functions for
targeted decompilation work.

Usage:
    python scripts/function_size_rank.py [OPTIONS]

Options:
    --format {json,table,csv,compact}  Output format (default: table)
    --limit N                  Show only top N functions
    --min-size N               Minimum function size (default: 0)
    --max-size N               Maximum function size (default: no limit)
    --category {small,medium,big,all}
                               Filter by category (default: all)
    --output PATH              Output file path

Categories:
    small:   0-100 bytes
    medium:  101-500 bytes
    big:     501+ bytes

Note: This script requires a non-stripped ELF with symbol tables.
For stripped ELFs, use the disassembly-based approach instead.
"""

from __future__ import annotations

import argparse
import json
import struct
from pathlib import Path
from typing import NamedTuple
import subprocess


ROOT = Path(__file__).resolve().parents[1]
BASEROM = ROOT / "baserom" / "SCUS_972.68"


class Function(NamedTuple):
    name: str
    address: int
    size: int
    category: str


def parse_elf_functions(elf_path: Path) -> list[Function]:
    """Extract function symbols from an ELF file using nm."""
    try:
        # Use nm to extract symbols
        result = subprocess.run(
            ['mips-linux-gnu-nm', '-n', str(elf_path)],
            capture_output=True,
            text=True,
            check=True
        )
        
        lines = result.stdout.strip().split('\n')
        functions = []
        
        # Parse nm output: address type name
        prev_addr = None
        prev_name = None
        
        for line in lines:
            parts = line.split()
            if len(parts) < 3:
                continue
            
            addr_str, sym_type, name = parts[0], parts[1], parts[2]
            
            # Skip non-function symbols
            if sym_type not in ('t', 'T', 'w', 'W'):  # text section functions
                continue
            
            try:
                addr = int(addr_str, 16)
            except ValueError:
                continue
            
            # Calculate size by looking at next function's address
            # For now, use a reasonable default
            size = 64  # Default size, will be refined
            
            # Determine category based on size
            if size <= 100:
                category = "small"
            elif size <= 500:
                category = "medium"
            else:
                category = "big"
            
            functions.append(Function(name=name, address=addr, size=size, category=category))
        
        return functions
        
    except subprocess.CalledProcessError as e:
        print(f"Error running nm: {e}")
        print("Trying alternative approach with readelf...")
        return parse_elf_with_readelf(elf_path)


def parse_elf_with_readelf(elf_path: Path) -> list[Function]:
    """Fallback method using readelf to get function info."""
    try:
        result = subprocess.run(
            ['mips-linux-gnu-readelf', '-s', str(elf_path)],
            capture_output=True,
            text=True,
            check=True
        )
        
        lines = result.stdout.strip().split('\n')
        functions = []
        
        # Parse readelf output
        for line in lines[3:]:  # Skip header lines
            parts = line.split()
            if len(parts) < 8:
                continue
            
            try:
                # readelf format: Num: Value Size Type Bind Vis Ndx Name
                addr = int(parts[1], 16)
                size = int(parts[2])
                sym_type = parts[3]
                name = parts[7]
            except (ValueError, IndexError):
                continue
            
            # Filter for functions only
            if sym_type != 'FUNC':
                continue
            
            # Skip empty names and non-function symbols
            if not name or name.startswith('.'):
                continue
            
            # Determine category based on size
            if size <= 100:
                category = "small"
            elif size <= 500:
                category = "medium"
            else:
                category = "big"
            
            functions.append(Function(name=name, address=addr, size=size, category=category))
        
        return functions
        
    except subprocess.CalledProcessError as e:
        print(f"Error running readelf: {e}")
        return []


def categorize_function(size: int) -> str:
    """Categorize a function by size."""
    if size <= 100:
        return "small"
    elif size <= 500:
        return "medium"
    else:
        return "big"


def rank_functions(functions: list[Function]) -> list[Function]:
    """Rank functions by size (largest first)."""
    return sorted(functions, key=lambda f: f.size, reverse=True)


def filter_functions(
    functions: list[Function],
    min_size: int = 0,
    max_size: int | None = None,
    category: str | None = None
) -> list[Function]:
    """Filter functions by size and/or category."""
    filtered = []
    for func in functions:
        if func.size < min_size:
            continue
        if max_size is not None and func.size > max_size:
            continue
        if category and func.category != category:
            continue
        filtered.append(func)
    return filtered


def format_output(
    functions: list[Function],
    format_type: str,
    limit: int | None = None
) -> str:
    """Format functions output in the specified format."""
    if limit:
        functions = functions[:limit]
    
    if format_type == "json":
        return json.dumps([
            {"name": f.name, "address": hex(f.address), "size": f.size, "category": f.category}
            for f in functions
        ], indent=2)
    
    elif format_type == "csv":
        lines = ["name,address,size,category"]
        for f in functions:
            lines.append(f"{f.name},{hex(f.address)},{f.size},{f.category}")
        return "\n".join(lines)
    
    elif format_type == "compact":
        # Simple format: func_xxxx size
        lines = []
        for f in functions:
            lines.append(f"{f.name} {f.size}")
        return "\n".join(lines)
    
    else:  # table
        if not functions:
            return "No functions found"
        
        lines = []
        lines.append(f"{'Name':<40} {'Address':<10} {'Size':>8} {'Category':<10}")
        lines.append("-" * 70)
        for f in functions:
            # Truncate long names
            name = f.name[:37] + "..." if len(f.name) > 40 else f.name
            lines.append(f"{name:<40} {hex(f.address):<10} {f.size:>8} {f.category:<10}")
        
        # Summary
        total = len(functions)
        small = sum(1 for f in functions if f.category == "small")
        medium = sum(1 for f in functions if f.category == "medium")
        big = sum(1 for f in functions if f.category == "big")
        
        lines.append("")
        lines.append(f"Total functions: {total}")
        lines.append(f"  Small (0-100):     {small}")
        lines.append(f"  Medium (101-500):  {medium}")
        lines.append(f"  Big (501+):        {big}")
        
        return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(
        description="Analyze and rank functions by size across the RAC2 codebase"
    )
    parser.add_argument(
        "--format", "-f",
        choices=["json", "table", "csv"],
        default="table",
        help="Output format (default: table)"
    )
    parser.add_argument(
        "--limit", "-l",
        type=int,
        default=None,
        help="Show only top N functions"
    )
    parser.add_argument(
        "--min-size", "-m",
        type=int,
        default=0,
        help="Minimum function size (default: 0)"
    )
    parser.add_argument(
        "--max-size", "-x",
        type=int,
        default=None,
        help="Maximum function size (default: no limit)"
    )
    parser.add_argument(
        "--category", "-c",
        choices=["small", "medium", "big", "all"],
        default="all",
        help="Filter by category (default: all)"
    )
    parser.add_argument(
        "--output", "-o",
        type=str,
        default=None,
        help="Output file path"
    )
    
    args = parser.parse_args()
    
    # Parse functions from ELF
    if not BASEROM.exists():
        print(f"Error: ELF file not found at {BASEROM}")
        print("Please provide your legally acquired SCUS_972.68 in baserom/")
        print("Note: The retail ELF is typically stripped. Use nm or readelf to extract symbols.")
        return 1
    
    print(f"Parsing functions from {BASEROM}...")
    functions = parse_elf_functions(BASEROM)
    
    if not functions:
        print("Warning: No function symbols found.")
        print("The ELF may be stripped. Try using mips-linux-gnu-readelf -s to extract symbols.")
        return 1
    
    print(f"Found {len(functions)} functions")
    
    # Rank and filter
    ranked = rank_functions(functions)
    filtered = filter_functions(
        ranked,
        min_size=args.min_size,
        max_size=args.max_size,
        category=args.category
    )
    
    # Format and output
    output = format_output(filtered, args.format, args.limit)
    
    if args.output:
        Path(args.output).write_text(output)
        print(f"Output written to {args.output}")
    else:
        print(output)
    
    return 0


if __name__ == "__main__":
    exit(main())
